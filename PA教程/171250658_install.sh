# !/bin/bash
Student_ID=171250658
Pwd=REFUdTVzbE54NHB6Q092dDJGSTMxS2tSaEdCTEpvZFU=


Server_IP=114.212.189.154
PA_name=pa2018_fall
PA_path=$(cd "$(dirname "$0")";pwd)

RED_COLOR='\E[1;31m'
GREEN_COLOR='\E[1;32m'
YELOW_COLOR='\E[1;33m'
BLUE_COLOR='\E[1;34m'
PINK='\E[1;35m'
RES='\E[0m'

User=`whoami` #get the current user executing the script

# cannot run as root
if [ $User = "root" ]; then
	echo "User Check FAIL: DO NOT run as root"
	exit
fi

echo -e "User Check ${GREEN_COLOR}PASS${RES}"
echo -e "Hello ${BLUE_COLOR}"$User"${RES}"

# check kernel release
KERNEL_RELEASE=`uname -r | grep -E '686'`
KERNEL_VER=`uname -v | grep -E '[[Dd]ebian|[Uu]buntu]'`
if [ -z "$KERNEL_RELEASE" ] || [ -z "$KERNEL_VER" ]; then
	echo -e "OS Check ${RED_COLOR}FAIL${RES}: 32-bit Debian Linux Required"
	exit
fi

echo -e "OS Check ${GREEN_COLOR}PASS${RES}"

echo -e "Enter your sudo password to install dependencies"
#resolve dependencies
Deps=(build-essential make libreadline-dev libsdl1.2-dev vim git tmux openssl dialog python)
sudo apt-get update
for dep in ${Deps[@]}; do
	sudo apt-get install $dep
	if [ -z '`dpkg-query -l | grep ${dep}`' ]; then
		echo -e "Dependency Check ${RED_COLOR}FAIL${RES}: ${dep} not installed"
		echo "Check your internet connection and manually install dependencies by the following command:"
		echo
		echo "sudo apt-get install build-essential libreadline-dev libsdl1.2-dev vim git tmux openssl dialog python"
		echo
		echo "Then execute this script again"
		exit
	fi
done
#set timezone
sudo cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
echo -e "Dependency Check ${GREEN_COLOR}PASS${RES}"

#setup git
echo -n "Please enter your email for git:"
read User_email
git config --global user.email "${User_email}"
echo -n "Please enter your name for git:"
read User_name
git config --global user.name "${User_name}"

echo -e "Git Config ${GREEN_COLOR}PASS${RES}"

#git clone
if [ ! -d $PA_path/$PA_name/ ]; then
	git config --global http.postBuffer 524288000
	git clone https://github.com/ics-pa/$PA_name.git
	if [ ! -d $PA_path/$PA_name/ ] || [ ! -f $PA_path/$PA_name/Makefile.git.temp ]; then
		echo -e "Git Clone ${RED_COLOR}FAIL${RES}"
		echo "Check your internet connection"
		echo "Solution 1: Execute the following command:"
		echo
		echo "1. rm -r -f ./"$PA_name"/"
		echo
		echo "2. Then execute this script again"
		echo
		echo "Solution 2: on Solution 1 keeps failing:"
		echo
		echo "1. rm -r -f ./"$PA_name"/"
		echo
		echo "2. In the same place you put your install.sh"
		echo
		echo "   Manually execute the following command and make sure your clone succeeded:"
		echo
		echo "   git clone https://github.com/ics-pa/${PA_name}.git"
		echo
		echo "3. Then execute this script again"
		exit
	fi
fi

if [ ! -f $PA_path/$PA_name/Makefile.git ]; then
	mv $PA_path/$PA_name/Makefile.git.temp $PA_path/$PA_name/Makefile.git
	sed -i "1a STU_ID = "$Student_ID $PA_path/$PA_name/Makefile.git
	sed -i "2a Server_IP = "$Server_IP $PA_path/$PA_name/Makefile.git
fi

echo -e "Git Clone ${GREEN_COLOR}PASS${RES}"

#make utility dirs
Util_path=/home/$User/.pa-nemu
if [ ! -d $Util_path ]; then
	mkdir $Util_path 
fi
if [ ! -d $Util_path"/scripts/" ]; then
	mkdir $Util_path"/scripts/"
fi
if [ ! -d $Util_path"/emotion/" ]; then
	mkdir $Util_path"/emotion/"
fi
if [ ! -d $Util_path"/tracking/" ]; then
	mkdir $Util_path"/tracking/"
fi
if [ ! -d $Util_path"/scripts/" ] || [ ! -d $Util_path"/emotion/" ] || [ ! -d $Util_path"/tracking/" ]; then
	echo -e "Deploy ${RED_COLOR}FAIL${RES}: Cannot create dirs"
	exit
fi

if [ ! -f $Util_path/scripts/do_not_touch_me2 ]; then
	echo $Pwd > $Util_path/scripts/do_not_touch_me
	echo $Student_ID > $Util_path/scripts/do_not_touch_me2
else
	that_id=`cat $Util_path/scripts/do_not_touch_me2`
	if [ $that_id != $Student_ID ]; then
		echo -e "${RED_COLOR}This is not your virtual machine! Do not run install.sh here!${RES}"
		exit
	fi
fi

#copy scripts for tracking
Script_dir=/home/$User/.pa-nemu/scripts/
Script_path=/home/$User/.pa-nemu/scripts/gitmonitor.sh
cp $PA_path/$PA_name/scripts/gitmonitor.sh $Script_path
cp $PA_path/$PA_name/scripts/upload_tracking ${Script_dir}upload_tracking
if [ ! -f $Script_path ]; then
	echo -e "Deploy ${RED_COLOR}FAIL${RES}: Cannot deploy tracking script"
	exit
fi
chmod 755 $Script_path
chmod 755 ${Script_dir}upload_tracking
chmod 755 $PA_path/$PA_name/scripts/update

#echo $Student_ID $Pwd $PA_PATH $User $Script_path
sed -i "1a Student_ID=$Student_ID" $Script_path
sed -i "2a User=$User" $Script_path
sed -i "3a PA_PATH=$PA_path/$PA_name" $Script_path
sed -i "4a Server_IP=$Server_IP" $Script_path

crontab_check=`cat /etc/crontab | grep -E "gitmonitor.sh"`
if [ -z "$crontab_check" ]; then
	echo -e "Enter your sudo password to deploy"
	sudo sed -i '$d' /etc/crontab
	sudo bash -c "echo -e '*/1 *   * * *  \c'>>/etc/crontab"
	sudo bash -c "echo $User $Script_path >>/etc/crontab"     
	sudo bash -c "echo '#'>>/etc/crontab"
	sudo /etc/init.d/cron restart
fi
echo -e "Deploy ${GREEN_COLOR}PASS${RES}"

echo -e "${GREEN_COLOR}Congratulations! You are ready to enjoy your Programming Assignment (PA)${RES}"
echo "Remember:"
echo "    * DO NOT share your install.sh with others"
echo "    * DO NOT let others know your environment configuration in your home folder"
echo "otherwise people can do anything to your account!"
