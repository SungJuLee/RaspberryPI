#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>      // O_RDWR , O_NOCTTY 등의 상수 정의 
 
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
 
#include <json/json.h>
 
void jsontest(char* buffer);
json_object* makeMsg();
 
int socket_connect(char *host, in_port_t port){
        struct sockaddr_in addr;
        int on = 1, sock;     
	struct hostent *server;
 
	bzero((char*)&addr,sizeof(addr));
        addr.sin_port = htons(port);
        addr.sin_family = AF_INET;
	server = gethostbyname(host);
	bcopy((char*)server->h_addr, (char*)&addr.sin_addr.s_addr,server->h_length);
        sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
        if(sock == -1){
                perror("setsockopt");
                exit(1);
        }
        if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1){
                perror("connect");
                exit(1);
 
        }
        return sock;
}
 
#define BUFFER_SIZE 1024
 
int main(int argc, char *argv[]){
        int fd;
        char buffer[BUFFER_SIZE];
	json_object *obj;

	obj = makeMsg();
	char* poststr = json_object_to_json_string(obj);

	//char *poststr="{\"flag\":\"101\",\"vehicle_code\":\"9999911\"}\r\n";

	char msg[1024];
	sprintf(msg,	"POST / HTTP/1.1\r\n"
			"Host: %s\r\n"
			"Connection: keep-alive\r\n"
			"Content-Type: Application/json\r\n"
			"Content-Length: %d\r\n\r\n"
			"%s\r\n", argv[1], strlen(poststr), poststr);
 
        // 포트를 open
        fd = socket_connect(argv[1], atoi(argv[2]));
 
 
        while (1) {
		printf("===================================Post Request\n%s",msg);
                write(fd, msg, strlen(msg));
                bzero(buffer, BUFFER_SIZE);
 
                if (read(fd, buffer, BUFFER_SIZE-1) == NULL) break;
		printf("===================================Response\n");
		printf("%s\n",buffer);

		char* data = strstr(buffer, "\r\n\r\n");
		printf("===================================Response End\n");
		printf("%d Data: %s\n", strlen(data+4), data+4);
		printf("===================================Data End\n");
		jsontest(data+4);
		bzero(buffer, BUFFER_SIZE);

		sleep(3);
        }
        close(fd);
 
 
        return 0;
}
 
void jsontest(char* buffer) {

        json_object *myobj;
        json_object *dval;

	printf("===Inside===\n");
	buffer = "{\"flag\": \"401\", \"vehicle_code\": \"999999\"}";
        myobj = json_tokener_parse(buffer);
	printf("===Inside2===\n");
	printf("%s\n",json_object_to_json_string(myobj));
        /* flag 영역 파싱 */
	dval = json_object_object_get(myobj,"flag");
	printf("===Inside3===\n");
	printf("%s\n",json_object_to_json_string(dval));
        printf("flag : %s\n", json_object_get_string(dval));
        //if (json_object_get_string(dval) == "401") {Msg401();}
	//if (json_object_get_string(dval) == "402") {Msg402();}
        //if (json_object_get_string(dval) == "499") {Msg499();}
        //if (json_object_get_string(dval) == "501") {Msg501();}
        //if (json_object_get_string(dval) == "502") {Msg502();}
        //if (json_object_get_string(dval) == "599") {Msg599();}

        //if (json_object_get_string(dval) == "301") {Msg301();}
        //if (json_object_get_string(dval) == "302") {Msg302();}
        //if (json_object_get_string(dval) == "303") {Msg303();}
        //if (json_object_get_string(dval) == "399") {Msg399();}
        //if (json_object_get_string(dval) == "601") {Msg601();}
        //if (json_object_get_string(dval) == "602") {Msg602();}
        //if (json_object_get_string(dval) == "603") {Msg603();}
        //if (json_object_get_string(dval) == "699") {Msg699();}


 
        /* vehicle_code 영역 파싱 */
        dval = json_object_object_get(myobj, "vehicle_code");
        printf("vehicle_code : %s\n", json_object_get_string(dval));
 
        /* raspberrypi_ip 영역 파싱 */
        dval = json_object_object_get(myobj, "raspberrypi_ip");
        printf("raspberrypi_ip : %s\n", json_object_get_string(dval));
 
         /* user_mac 영역 파싱 */
        dval = json_object_object_get(myobj, "user_id");
        printf("user_id : %s\n", json_object_get_string(dval));
 
        /* user_mac 영역 파싱 */
        dval = json_object_object_get(myobj, "user_mac");
        printf("user_mac : %s\n", json_object_get_string(dval));

	printf("===END===\n\n");
}

json_object* makeMsg() {
	json_object* obj;

	obj = json_object_new_object();

	json_object_object_add(obj, "flag", json_object_new_string("101"));
	json_object_object_add(obj, "vehicle_code", 
		json_object_new_string("000000000000"));
	json_object_object_add(obj, "user_id",
		json_object_new_string("aaaaaaa@gmail.com"));
	json_object_object_add(obj, "user_mac",
		json_object_new_string("55:55:55:55:55:11"));

	//json_object_put(obj);

	return obj;
}
