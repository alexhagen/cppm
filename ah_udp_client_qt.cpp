#include "ah_udp_client_qt.h"
#include <QDebug>

ah_udp_client_qt::ah_udp_client_qt(void){
    cont = true;
    qDebug() << "starting socket";
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);
    addrlen = sizeof(addr);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }
    mreq.imr_multiaddr.s_addr = inet_addr(GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
        &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt mreq");
        exit(1);
    }
}

void ah_udp_client_qt::loop(void) {
    cnt = recvfrom(sock, message, sizeof(message), 0,
        (struct sockaddr *) &addr, &addrlen);
    if (cnt < 0) {
        printf("error");
        perror("recvfrom");
        exit(1);
    } else if (cnt > 0) {
        emit data_ready(message);
    }
}

void ah_udp_client_qt::start(void) {
    while (cont) {
        loop();
    }
};

void ah_udp_client_qt::start(int t) {
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000*t);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start();
}

