#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <memory>
#include <vector>
#include <list>

using namespace std;
#include "control_pipe.h"
#include "datastruct.h"
#include "files.h"
#include "player.h"
#define CTL_BUFFER_SIZE 300

FILE *f_ctl;
extern playbackStatus ps;
extern settings s;

extern list<string> pqueue;

/*
 * Opens a file (pipe) to be used to control the RDS coder, in non-blocking mode.
 */

int open_control_pipe(const char *filename) {
	int fd = open(filename, O_RDONLY | O_NONBLOCK);
    if(fd == -1) return -1;

	int flags;
	flags = fcntl(fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if( fcntl(fd, F_SETFL, flags) == -1 ) return -1;

	f_ctl = fdopen(fd, "r");
	if(f_ctl == NULL) return -1;

	return 0;
}


/*
 * Polls the control file (pipe), non-blockingly, and if a command is received,
 * processes it and updates the RDS data.
 */

int poll_control_pipe() {
    static char buf[CTL_BUFFER_SIZE];
    char *res = fgets(buf, CTL_BUFFER_SIZE, f_ctl);
    string::size_type sz;
    if(res == NULL) return -1;

    string input = string(res);
    input.erase(input.size() - 1);	//removing carriage return

    size_t found = input.find_first_of(" ");
    cout<<"found space at: "<<found<<endl;
    string command = input.substr(0,found);
    string arguments = input.substr(found+1);
    cout<<"command: "<<command<<" arguments: "<<arguments<<endl;

    if (command.compare("SKIP") == 0){
	    killpg(ps.pid,15);
    }else if(command.compare("SEEK") == 0){
	    if(!(arguments[0] == '+' || arguments[0] == '-')) return -1;

	    ps.reloading = true;

	    ps.playbackPosition = ps.playbackPosition + stoi(arguments,&sz);
	    if(ps.playbackPosition < 0)
		    ps.playbackPosition = 0;

	    s.resumePlayback = true;
	    ps.resumed = false;
	    killpg(ps.pid,15);
    }/*else if(command.compare("PLAY") == 0){
    	if(!(arguments[0] == '/')) return -1;
		ps.songPath = arguments;
		read_tag_to_status(ps.songPath);
		ps.reloading = true;
		ps.playbackPosition = 0;
		cout<<"Old Index : "<<ps.songIndex <<endl;
		ps.songIndex = getindex(ps.songPath);
		get_file_format(arguments);
		s.resumePlayback = true;
		ps.resumed = false;
		update_now_playing();
		cout<<"New Index : "<<ps.songIndex <<endl;
		killpg(ps.pid,15);
    }*/else if(command.compare("SCAN") == 0){
        if(arguments[0] != '/') return -1;
        s.storage=arguments;
        media_scan();
		set_next_element();
        ps.reloading=true;
        killpg(ps.pid,15);
    }
    else if(command.compare("PLAY") == 0){
    	    if(!(arguments[0] == '/')) return -1;
    	    ps.songPath = arguments;
    	    read_tag_to_status(ps.songPath);
    	    ps.reloading = true;
    	    ps.playbackPosition = 0;
    	    ps.songIndex = getindex(ps.songPath);
    //	    ps.songName = ps.songIndex +ps.songName;
    	    get_file_format(arguments);
        	s.resumePlayback = true;
    	    ps.resumed = false;
    	    update_now_playing();
    	    killpg(ps.pid,15);
    }
    else if(command.compare("PREV") == 0){
    		ps.playbackPosition = 0;
    		if(ps.songIndex == 0)
    		{
    			ps.songIndex = pqueue.size()-1;
    		}
    		else
    		{
    			ps.songIndex = ps.songIndex -1;
    		}
            ps.songPath = get_file_at_index(ps.songIndex);
			read_tag_to_status(ps.songPath);
    		ps.reloading = true;
    		s.resumePlayback = true;
    		ps.resumed = false;
    		get_file_format(ps.songPath);
    		update_now_playing();
    		killpg(ps.pid,15);
    }
    else if(command.compare("BT") == 0){
             if(arguments[0] != '/') return -1;
             ps.songPath = "BLUETOOTH";
         	ps.songArtist = "";
         	ps.songAlbum = "";
         	ps.songYear = "";
         	ps.AlbumArt = "";
             update_now_playing();
            /* cout<<"Stopping Playback"<<endl;
             ps.stop = true;
             ps.reloading = true;
             killpg(ps.pid,15);
             //system(("systemctl start mpradio-bt@"+arguments).c_str());
             //system(("killall mpradio && killall sox").c_str());
             cout<<"Playing Bluetooth from : "<< arguments  << endl;
             play_bt(arguments);*/
             //system(("/home/pi/play-bt.sh "+arguments).c_str());
             system(("/var/play-bt.sh "+arguments).c_str());
     }
    else if(command.compare("RT") == 0){
        if(arguments[0] != '/') return -1;
       /* cout<<"Stopping Playback"<<endl;
        ps.stop = true;
        ps.reloading = true;
        killpg(ps.pid,15);
        //system(("systemctl start mpradio-bt@"+arguments).c_str());
        //system(("killall mpradio && killall sox").c_str());
        cout<<"Playing Bluetooth from : "<< arguments  << endl;
        play_bt(arguments);*/
        ps.songPath = arguments;
		ps.songArtist = "";
		ps.songAlbum = "";
		ps.songYear = "";
		ps.AlbumArt = "";
        update_now_playing();
    }
    else if(command.compare("STOP") == 0){
        if(arguments[0] != '/') return -1;
        system(("systemctl stop mpradio").c_str());
    }

    //ps.reloading = false;
    return 0;
}

int close_control_pipe() {
    if(f_ctl) return fclose(f_ctl);
    else return 0;
}

//template<class T>
//shared_ptr<vector<T>>
//ListToVector(list<T> List) {
//shared_ptr<vector<T>> Vector {
//        new vector<string>(List.begin(), List.end()) }
//return Vector;
//}
