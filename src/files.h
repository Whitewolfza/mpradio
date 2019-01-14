#pragma once

void save_list(int qsize);
void media_scan();
void load_saved_list();
int get_file_size(string filename);
int get_file_bs(int filesize,float fileduration);
float get_song_duration(string path);
void load_playback_status();
void update_playback_status();
void update_now_playing();
void read_tag_to_status(string songpath);
void get_file_format(string songpath);
void control_pipe_setup();
int getindex(string songpath);
string get_album_art(string songpath);
string get_file_at_index(int index);
