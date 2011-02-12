#include "pente_com.h"

Pente_Com::Pente_Com()
	  :n_read(0), n_write(0){
    char buffer[8];

    read( STDIN_FILENO, buffer, sizeof(buffer));

    if( strcmp( buffer, "001WHTE") == 0){
	color = WHITE;
	n_read++;
    }
    else if( strcmp( buffer, "001BLCK") == 0){
	color = BLACK;
	n_read++;
    }
}

Pente_Com::~Pente_Com(){
    //nothing needed at the moment
}

player Pente_Com::get_color(){
    return color;
}

void Pente_Com::play_move( Move_t move_to_play){ //incomplete
    /* the buffer[8] format is: NNNXXYY'\0' where NNN is the nth communication
     * XX is the x coords, YY is the y coords */
    char buffer[8];
    n_write++;

    buffer[0] = (n_write / 100) + '0';
    buffer[1] = ((n_write % 100) / 10) + '0';
    buffer[2] = (n_write % 10) + '0';
    buffer[3] = (move_to_play.x / 10) + '0';
    buffer[4] = (move_to_play.x % 10) + '0';
    buffer[5] = (move_to_play.y / 10) + '0';
    buffer[6] = (move_to_play.y % 10) + '0';
    buffer[7] = '\0';

    write( STDOUT_FILENO, buffer, sizeof(buffer));
}

Move_t Pente_Com::get_opponents_move(){ //incomplete
    Move_t to_return;
    to_return.x = 0;
    to_return.y = 0;
    int temp_read = 0;
    char buffer[8];

//    sleep(1);

    read( STDIN_FILENO, buffer, sizeof( buffer));

    /* the buffer[8] format is: NNNXXYY'\0' where NNN is the nth communication
     * XX is the x coords, YY is the y coords */
    temp_read += ( 100 * (buffer[0] - '0'));
    temp_read += ( 10 * (buffer[1] - '0'));
    temp_read += buffer[2] - '0';

    while( temp_read == n_read){
//	sleep(1);

	read( STDIN_FILENO, buffer, sizeof( buffer));
	temp_read = 0;
	temp_read += ( 100 * (buffer[0] - '0'));
	temp_read += ( 10 * (buffer[1] - '0'));
	temp_read += buffer[2] - '0';
    }

    if( temp_read == n_read + 1){
	to_return.x += ( 10 * (buffer[3] - '0'));
	to_return.x += buffer[4] - '0';
	to_return.y += ( 10 * (buffer[5] - '0'));
	to_return.y += buffer[6] - '0';
	n_read++;
    }
    else if( temp_read > n_read + 1){
	exit(-1);
    }

    return to_return;
}
