#include "libchatnet.h"

#define CHATNETKW "chatnet"
#define chatnet str_addva(MGN, "[chatnet]", R0)

	//	printf();
	//	printf("%s------------------------------------%s\n", BLU, R0);
void __printfAllCmds__() {printf(
	"%s--------%sCHATNET COMMANDS%s--------%s\n"
	"%sCommands%s    %sDescription%s\n"
	"list        List all active users in the network\n"
	"read        Starts reading all your messages\n"
	"write       Starts input prompt for writing messages\n"
	"exit        Exits from the Chatnet network\n"
	"\n"
	"[Example]\n"
	"%s >> chatnet list\n"
	"%s >> chatnet read\n"
	"%s >> chatnet write\n"

	"%s--------------------------------%s\n"
	
	, BLU, GRN, BLU, R0
	, GRY, R0, GRY, R0

	




	, read_uSend()
	, read_uSend()
	, read_uSend()
	, BLU, R0);
}

void notice(const char* what);
void __printfMsgExample__();
void chatnet_execCmd(char* msgText);
int isActive_Valid_uRecv(const char* uRecv);

void chatnet_init() {
	if (! dir_exists(cdir)) mkdir(cdir, 0755);


	if (! file_exists(uSendDir)) {
		char* uSend = input("[Init] Enter username: ");
		file_write(uSendDir, uSend);
	}

	
	__printfAllCmds__();


	//TODO ASCII Art
	//if (! file_exists(uRecvAllFn)) file_write(uRecvAllFn, "");
}


void chatnet_read() {
	notice("read_AllMsg");
	file_write(readingAlreadyFn, "");
	while (1) {
		char* MsgAll = read_AllMsg();
		if (str_eq(MsgAll, "")) {	
			//failed, no msg recvd
			sleep(1);
			//printf("--nothing received\n");
		}
		else {
			printf("%s", MsgAll);
			strcpy(MsgAll, "");
		}
	}
}


void chatnet_write() {
	notice("write_ThisMsg");
	char* uSend = read_uSend();
	
	while (1) {
		char* msgText = input(str_addva(uSend," >> "));
		int canWrite = true; // Every baby is born Muslim (submitting to the best words of Allah).
		
		char* uRecv = read_uRecvFromMsg(msgText);
		if (str_eq(uRecv, CHATNETKW)) {
			chatnet_execCmd(msgText); 
			canWrite = false;
		}


		if (canWrite == true && isActive_Valid_uRecv(uRecv) == false) { //true & true 
			notice("inactive_invalid_uRecv");
			__printfMsgExample__();
			canWrite = false;
		}
		if (canWrite == true && file_exists(str_addva(shkeyDir, "/", uRecv)) == false) {
			notice("write_chatroom");
			write_chatroom(uRecv);
		}
		if (canWrite == true) write_ThisMsg(msgText);
	}
}


void chatnet_execCmd(char* msgText) {
	int lenKeyword = (int)strlen(CHATNETKW);
	//int nextSpace = str_index(msgText, " ", lenKeyword + 1, strlen(msgText));
	char* cmd = str_slice(msgText, lenKeyword + 1, 1, strlen(msgText));
	printf("%s Executing chatnet-native command: %s\n", info, cmd);

	if      (str_eq("exit", cmd)) write_exit();
	else if (str_eq("list", cmd)) printf("%s", read_active());
	else if (str_eq("read", cmd)) chatnet_read();
	else if (str_eq("write", cmd)) chatnet_write();
}



void __printfMsgExample__() {printf("%s[Example]%s %sNameOfUser%s The first word in every message is the receiver's username. In this case the receiver is %s'NameOfUser'%s\n", MGN, R0, CYN, R0, CYN, R0);}


void notice(const char* about) {
	// see that's how you encapsulate in C, object-orientation in C
	if (str_eq(about, "write_chatroom")) {
		printf("%s Creating new chatroom...\n", info);
	}
	else if (str_eq(about, "read_AllMsg")) {
		printf("%s Please run Chatnet.exe again to write messages.\n", warn);
		__printfMsgExample__();
		printf("%s Starting to receive all messages in this terminal window.\n", info);
	}
	else if (str_eq(about, "inactive_invalid_uRecv")) {
		printf("%s[Message not send]%s Either uRecv %s(the first word in your message)%s is invalid %s(contains anything other than alphabet)%s, or the recipient user is %sinactive%s right now.\n", RED, R0, GRY, R0, GRY, R0, GRY, R0);
	}
}


int isActive_Valid_uRecv(const char* uRecv) {
	/*char* uRecvAll = file_read(uRecvAllFn);
	if (str_index(uRecvAll, str_addva("_", uRecv, "_"), 0, strlen(uRecvAll)) != -1)
		return 1; //true
	else return 0;
	*/
	char* activeAll = read_active();
	char* _uRecv_ = str_addva("[+] ", uRecv, "\n");
	if (str_index(activeAll, _uRecv_, 0, strlen(activeAll)) != -1) {
		// uRecv is active
		if (str_isalpha(uRecv)) return 1; //true. Perfect!
		else return 0;
	}
	else return 0;
}


int main () {
	chatnet_init();
	
	/*
	if (! file_exists(readingAlreadyFn)) {
		chatnet_read();
	}
	else chatnet_write();
	*/
	chatnet_write();
}