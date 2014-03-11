#ifndef MSGECHO_H
#define MSGECHO_H
int get_callbackmessagelen(void);
void init_callbackmessage(void);
void get_callbackmessage(char *p, int len);
void reset_callbackmessage(void);

int get_headframelen(void);
void set_headframedata(char *p,int len);

int get_dataframelen(void);
#endif // MSGECHO_H
