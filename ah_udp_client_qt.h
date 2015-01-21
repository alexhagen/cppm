#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QObject>
#include <QTimer>

#define PORT 6000
#define GROUP "239.1.1.1"

#define BUFSIZE 2048

class ah_udp_client_qt : public QObject {

    Q_OBJECT

public:
    ah_udp_client_qt(void);
    void start(void);
    void start(int);

public slots:
    void loop(void);

signals:
    void data_ready(char* msg);

private:
    struct sockaddr_in addr;
    socklen_t addrlen;
    int sock, cnt;
    struct ip_mreq mreq;
    char message[BUFSIZE];
    bool cont;
};
