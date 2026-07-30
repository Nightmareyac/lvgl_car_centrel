#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    char *tittle;
    char *songer;
    char *path;         //音频路径
    char *cover_path;   //封面路径
    char *cover_path1;
    int   duration;     //时长（秒）
}Song;

extern Song song_list[];
extern int song_count;

void music_init(void);
void music_play_toggle(void);
void music_next(void);
void music_prev(void);
void music_jump_to(int index);
void music_set_volume(int percent);
int  music_get_progress(void); // 返回0-100

Song *music_get_current_song(void);
bool music_is_playing(void);


#endif

