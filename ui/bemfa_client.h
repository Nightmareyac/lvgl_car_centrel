#ifndef BEMFA_CLIENT_H
#define BEMFA_CLIENT_H

typedef struct
{
    bool trunk_open;
    bool lock_locked;
    bool ac_on;
    bool is_updated;
    bool frunk_open;
    bool flash1_turn; 
    bool flash2_turn;
    bool flash3_turn;
} BemfaDeviceState;

extern BemfaDeviceState g_bemfa_state;

int bemfa_connect(void);

void bemfa_send_msg(const char * topic , const char * msg);

void bemfa_disconnect(void);

#endif