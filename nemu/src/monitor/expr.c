/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include "monitor/expr_helper.h"
//For the case of * and -   we could see whether the token before it is the NUM
//If that is NUM. * means multiple, and - means sub.
// or * means the ptr, - means NEG

enum
{
	NOTYPE = 256,
	/*
	When the value of the type is lagerer than the 256,
	It will be the operator
	*/
	NUM,
	HEX,
	REG,
	SYMB,
	/*
	Or it is the operand (+ - * / )
	*/
	MEM_ptr = -2,
	NEG = -1,
	EQ,
	UEQ,
	AND,
	OR,
	NOT = -3, // !
			  /* TODO: Add more token types */

};

struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // white space
	{"\\+", '+'},
	{"-", '-'},
	{"\\*", '*'},
	{"/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"==", EQ},
	{"!=", UEQ},
	{"&&", AND},
	{"\\|\\|", OR},
	{"!", NOT},
	{"0[xX][0-9a-fA-F]+", HEX}, //0xFFF
	{"[0-9]+", NUM},
	{"\\$e[a,c,d,b]x", REG},
	{"\\$esp", REG},
	{"\\$esi", REG},
	{"[a-zA-Z]+[a-zA-Z0-9]*", SYMB}, // symbol of the virable.
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32]; //One token arr to put the token of the expr
int nr_token;	 //The number of the tokens in the expr
static bool is_pre_operand(uint32_t index_of_arr,
						   Token *tokens)
{
	if (index_of_arr == 0) //The first one.
		return false;
	Token t = tokens[index_of_arr - 1];
	if (t.type == ')')
		return true;
	return is_operand(t.type);
}
bool make_token(char *e)
{
	int position = 0;
	int i; //Index of the rules[]
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			bool size_validity = true;
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position; //The begin of the target token
				int substr_len = pmatch.rm_eo;
				printf("match regex[%d] at position %d with len %d: %.*s\n",
					   i, position, substr_len, substr_len, substr_start);

				position += substr_len; //Set the position to the next pos
				switch (rules[i].token_type)
				{
				case NOTYPE: //skip
					break;
				case '*':
					if (!is_pre_operand(nr_token, tokens))
						tokens[nr_token].type = MEM_ptr;
					else
						tokens[nr_token].type = '*';
					nr_token++;
					break;
				case '-':
					if (!is_pre_operand(nr_token, tokens))
						tokens[nr_token].type = NEG;
					else
						tokens[nr_token].type = '-';
					nr_token++;
					break;
				case NUM:
				case HEX:
				case REG:
				case SYMB:
					tokens[nr_token].type = rules[i].token_type;
					size_validity = put_val_str(tokens[nr_token].str,
												substr_start, substr_len);
					nr_token++;
					break;
				default: // operand + - * / ( )  !
					tokens[nr_token].type = rules[i].token_type;
					nr_token++;
					break;
				}
				break;
			}
			//If the size of the operand is lagerer than 32.
			if (!size_validity)
			{
				printf("operand unvalid!\n");
				return false;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}
/**
 *  @param: *e means the input string
 * 	@param: *success means whether the operation is right.
 * 	@return: the value of the result; 
 * 
*/
uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}
	return eval(0, nr_token - 1, success);
}

uint32_t eval(int begin, int end, bool *success)
{
	if (begin > end)
	{
		*success = false;
		return 0;
	}
	/*
	Single token
	return the value.
	*/
	else if (begin == end)
	{
		//compute the value of the number
		*success = true;
		char *num_str = tokens[begin].str;
		return val_fetch(num_str, tokens[begin].type);
	}
	// else if (begin + 1 == end)
	// {
	// 	char *num_str = tokens[end].str;
	// 	uint32_t val = val_fetch(num_str, tokens[end].type);
	// 	switch (tokens[begin].type)
	// 	{
	// 	case MEM_ptr:
	// 		return vaddr_read(val, SREG_SS, 32);
	// 	case NEG:
	// 		return -val;
	// 	default:
	// 		assert(0);
	// 	}
	// }
	else if (check_parentheses(begin, end) > 0)
	{
		/*
		The expr is surrounded by a pair of parentheses.
		*/
		return eval(begin + 1, end - 1, success);
	}
	else if (check_parentheses(begin, end) < 0)
	{
		assert(0);
	}
	else
	{
		uint32_t val_left = 0;
		uint32_t op = dominant_op(begin, end);
		// printf("op=%d\n", op);
		// printf("op tokens type val=%d\n", tokens[op].type);
		if (tokens[op].type >= 0)
		{
			// printf("op-1 = %d\n", op - 1);
			val_left = eval(begin, op - 1, success);
		}
		uint32_t val_right = eval(op + 1, end, success);
		if (!*success)
			return 0;
		switch (tokens[op].type)
		{
		case '+':
			return val_left + val_right;
		case '-':
			return val_left - val_right;
		case '*':
			return val_left * val_right;
		case '/':
			return val_left / val_right;
		case EQ:
			return val_left == val_right;
		case UEQ:
			return val_left != val_right;
		case AND:
			return val_left && val_right;
		case OR:
			return val_left || val_right;
		case MEM_ptr:
			return vaddr_read(val_right, SREG_SS, 1);
		case NEG:
			return -val_right;
		case NOT:
			return ~val_right;
		default:
			assert(0);
		}
	}
}

uint32_t val_fetch(char *str, int type)
{
	switch (type)
	{
	case NUM:
		return get_num(str);
	case HEX:
		return get_hex(str);
	case REG:
		return get_reg(str);
	case SYMB:
		return get_symbol(str);
	default:
		assert(0);
	}
}
uint32_t dominant_op(int begin, int end)
{
	int parentheses_counter = 0;
	int result_pos = -1;
	uint32_t priority;
	for (int index = begin; index <= end; index++)
	{
		Token t = tokens[index];
		if (is_operand(t.type))
		{
			continue;
		}
		if (t.type == '(')
		{
			parentheses_counter++;
		}
		else if (t.type == ')')
		{
			parentheses_counter--;
		}
		else if (parentheses_counter == 0)
		{
			priority = operator_priority(index);
			if (result_pos < 0)
			{
				result_pos = index;
			}
			else
			{
				uint32_t result_pro = operator_priority(result_pos);
				result_pos =
					(result_pro >= priority) ? index : result_pos;
			}
		}
		else if (parentheses_counter != 0 && index == end)
		{
			printf("parenthesses error!\n");
			assert(0);
		}
	}
	return result_pos;
}

bool is_operand(int type)
{
	return type > NOTYPE;
}
uint32_t operator_priority(uint32_t index)
{
	Token t = tokens[index];
	uint32_t result;
	switch (t.type)
	{
	case '(':
	case ')':
		result = 1;
		break;
	case MEM_ptr:
	case NOT:
	case NEG:
		result = 2;
		break;
	case '*':
	case '/':
		result = 3;
		break;
	case '+':
	case '-':
		result = 4;
		break;
	case EQ:
	case UEQ:
		result = 7;
		break;
	case AND:
		result = 11;
		break;
	case OR:
		result = 12;
		break;
	default:
		printf("op val=%d\n", t.type);
		assert(0);
	}
	return 15 - result;
}

int check_parentheses(int begin, int end)
{
	int result = 1;
	int counter = 0;
	for (int index = begin; index <= end; index++)
	{
		if (tokens[index].type == '(')
		{
			counter++;
		}
		else if (tokens[index].type == ')')
		{
			counter--;
		}
		if (result == 1 && counter == 0 && index < end)
			result--;
	}
	return counter == 0 ? result : -1;
}