#include <QCoreApplication>
#include <pthread.h>
#include <iostream>

using namespace std;

void *PrintHello(void *threadid) {
    long tid = (long) threadid;
    cout << "Hello World! Theard ID, " << tid << endl;
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[4];
    int rc;
    for(int i = 0; i < 4; i++) {
        cout << "main() Creating Thread :D" << i << endl;
        rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);
        if(rc) {
            cout << "Unable to creeate thread!" << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
