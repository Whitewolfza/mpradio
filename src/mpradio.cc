/**
 * \mainpage mpradio
 *
 * mpradio implements PiFmRDS, please check out their github and license.
 *
 * @author Moreno Razzoli
 */

/**
 * @file 
 * mpradio main
 */

#include <iostream>
using namespace std;

#include "player.h"
#include "settings_provider.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

/**
 * the main() checks whether an argument is passed or not
 * if an argument is passed, that is intended to be the 
 * bluetooth device to stream from. 
 * if no parameter is passed, mpradio will play mp3s 
 * from the folder specified in settings ( using ::getsettings )
*/
int main(int argc, char* argv[])
{
	system("sudo fsck /pirateradio/ -a");
	usleep(3000 * 1000);
	getsettings();
	if(argc>1){
		cout<<argv[1]<<endl;
		play_bt(argv[1]);
	}else{
		cout<<"no bluetooth device provided, playing mp3s..."<<endl;
		play_storage();
	}
	/*if(findBluetooth()){
		play_bt(argv[1]);
	}
	else
	{
		cout<<"no bluetooth device provided, playing mp3s..."<<endl;
		play_storage();
	}*/

	//play_storage();
	return 0;
}


/*bool findBluetooth()
{
	 inquiry_info *ii = NULL;
	    int max_rsp, num_rsp;
	    int dev_id, sock, len, flags;
	    int i;
	    char addr[19] = { 0 };
	    char name[248] = { 0 };

	    dev_id = hci_get_route(NULL);
	    sock = hci_open_dev( dev_id );
	    if (dev_id < 0 || sock < 0) {
	        perror("opening socket");
	        exit(1);
	    }

	    len  = 8;
	    max_rsp = 255;
	    flags = IREQ_CACHE_FLUSH;
	    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));

	    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, flags);
	    if( num_rsp < 0 ) perror("hci_inquiry");

	    for (i = 0; i < num_rsp; i++) {
	        ba2str(&(ii+i)->bdaddr, addr);
	        memset(name, 0, sizeof(name));
	        if (hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),
	            name, 0) < 0)
	        strcpy(name, "[unknown]");
	        printf("%s  %s\n", addr, name);
	    }

	    free( ii );
	    close( sock );
	    return 0;
		ofstream myfile;
		  myfile.open ("/pirateradio/bt.txt");
		  myfile << device_info;
		  myfile.close();
	return false;
}
}*/
