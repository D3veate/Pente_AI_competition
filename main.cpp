#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "board.h"
using namespace std;

player parent_process(Board&, int[], int[], int[], int[], bool, int);
void child_process(const char*, int[], int[]);
Move wait_for_move( int&, int);
void send_move( int&, int, Move);

int main(int argc, char** argv){
    /*vars*/
    /* for process communication using dup2() and read/write */
    int out_filedes_child_one[2];
    int out_filedes_child_two[2];
    int in_filedes_child_one[2];
    int in_filedes_child_two[2];

    int stdout_restore; //restores the stdout filestream for this process only
    stdout_restore = dup(STDOUT_FILENO);

    /* these strings capture the info int ./info.txt */
    string program_one, program_two;
    string visual;
    const char* program_one_cstr;
    const char* program_two_cstr;

    /*grab names of competing AI's*/
    ifstream infile("./info.txt");
    if(infile == NULL){
	cerr << "Couldn't open ./info.txt" << endl;
	exit(-1);
    }

    infile >> program_one >> program_two >> visual;
    infile.close();

    cerr << program_one << " : " << program_two << endl;

    bool visual_flag; //Used to determine whether to show each move.//
    visual_flag = (strcmp( visual.c_str(), "visual") == 0) ? true : false;

    /* converting string -> const char* so that the eventual call to execl won't be confused */
    program_one_cstr = program_one.c_str();
    program_two_cstr = program_two.c_str();

    /* these keep track of the spawned and current pid's. TODO: use these to make sure
     * that the child processes terminate. */
    pid_t parent_pid, child_one_pid, child_two_pid;
    parent_pid = getpid();

    /* This command sets up all 4 channels of communication that main.cpp cares about.
     * All of the pipes are half-duplex. */
    if( pipe( out_filedes_child_one) < 0 || pipe( out_filedes_child_two) < 0
     || pipe( in_filedes_child_one) < 0 || pipe( in_filedes_child_two) < 0 ){
	cerr << "Program communication failed to initialize" << endl;
	exit(-1);
    }

    /* Create the first of two child processes */
    child_one_pid = fork();

    if( child_one_pid == 0){ /*CHILD ONE PROCESS*/
	child_process( program_one_cstr, out_filedes_child_one, in_filedes_child_one);
	  //child_process() will initialize the pipes and spawn the first program listed in
	  //info.txt
    }
    else if( child_one_pid > 0){ //parent process of first fork
    }
    else{
	cerr << "Fork error (1)" << endl;
	exit(-1);
    }

    /* create second of two child processes */
    child_two_pid = fork();

    if( child_two_pid == 0){ /*CHILD TWO PROCESS*/
	child_process( program_two_cstr, out_filedes_child_two, in_filedes_child_two);
    }
    else if( child_two_pid > 0){
    }
    else{
	cerr << "Fork error (2)" << endl;
	exit(-1);
    }

    /* Only the parent process will reach this part of the program. */
    player winner;
    Board the_board;

    /* PARENT PROCESS */
    if( parent_pid == getpid()){ //Make CERTAIN that only the parent process executes the following code.//
	winner = parent_process(the_board, out_filedes_child_one, in_filedes_child_one, out_filedes_child_two, in_filedes_child_two, visual_flag, stdout_restore); //parent_process() coordinates the back and forth play. Both
					  //visual_flag and the_board are passed by reference (alias)
    }

    if( visual_flag){
	dup2( stdout_restore, STDOUT_FILENO); //Restore the stdout stream. Again, parent process only. 
	the_board.display();

	if( winner == WHITE){
	    cout << "The winner is " << program_one << endl;
	}
	else{
	    cout << "The winner is " << program_two << endl;
	}
    }

    //TODO: Make sure that the child processes have executed here. Terminate them if necessary. 

    return 0;
}

player parent_process(Board& the_board, int out_child_one[], int in_child_one[], int out_child_two[], int in_child_two[], bool visual_flag, int stdout_restore){
    /* Keep track of how many communications have been sent/received to detect communication errors.*/
    int read_one = 0, read_two = 0, sent_one = 0, sent_two = 0;
    Move temp;

//    sleep(1); //Encourage the child processes to both start. This ought ot only be a problem if an energetic
	      //multi-core system got ahead of itself. I hope. 

    /* Closing the portion of the pipes that we don't need to worry about in the parent process. */
    if( close( out_child_one[0]) != 0 || close( in_child_one[1]) != 0 || close( out_child_two[0]) != 0 || close( in_child_two[1]) != 0){
	cerr << "Problem with creating half-duplex pipes." << endl;
	exit(-1);
    }

    /* Send signals needed to initialize the Pente_Com class objects. These
     * two messages dictate how many messages have been sent to the respective
     * child process and what color that process will play as. */
    write( out_child_one[1], "001WHTE\0", 8); //Note: eight chars are ALWAYS sent when
					      //write() is used. This is a precaution.
    sent_one++;
    write( out_child_two[1], "001BLCK\0", 8);
    sent_two++;

    while( !the_board.get_game_over() && sent_one < 300){

	if( the_board.get_player() == WHITE){
	    temp = wait_for_move( read_one, in_child_one[0]); //receive from white
	    send_move( sent_one, out_child_two[1], temp); //send to black
	    //the_board.make_move( WHITE, temp);
	}
	else{
	    temp = wait_for_move( read_two, in_child_two[0]); //receive from black
	    send_move( sent_two, out_child_one[1], temp);  //send to white

	    //the_board.make_move( BLACK, temp);
	}

	if( the_board.move_is_legal( the_board.get_player(), temp)){
	    the_board.make_move( the_board.get_player(), temp);
	}
	else{
	    the_board.display(true); //Display the board using cerr
	    cerr << "Illegal move ( " << the_board.get_player() << " ) " << " -- x: " << temp.x << " y: " << temp.y << endl;
	}
    }

    return the_board.get_winner();
}

void child_process(const char* file_name, int out_filedes[2], int in_filedes[2]){
    //keep in mind, the descriptions in/out are relative to the parent process
    if( close( out_filedes[1]) != 0 || close( in_filedes[0]) != 0){ 
	cerr << "Problem with creating half-duples pipes (2)." << endl;
	exit(-1);
    }

    if( dup2( out_filedes[0], STDIN_FILENO) != STDIN_FILENO){
	cerr << "Error duplicating STDIN file descriptor." << endl;
	exit(-1);
    }

    if( dup2( in_filedes[1], STDOUT_FILENO) != STDOUT_FILENO){
	cerr << "Error duplicating STDOUT file descriptor." << endl;
	exit(-1);
    }

    if( execl( file_name, file_name, NULL) < 0){
	cerr << "Error with calling: " << file_name << endl;
	exit(-1);
    }
}

Move wait_for_move( int& last_com, int read_filedes){
    int read_temp = 0;
    Move to_return;
    char buffer[8];

    do{
	read( read_filedes, buffer, sizeof(buffer));

	read_temp = 0;
	read_temp += ( 100 * (buffer[0] - '0'));
	read_temp += ( 10 * (buffer[1] - '0'));
	read_temp += buffer[2] - '0';
    }while( last_com == read_temp /*&& sleep(1) == 0 */); //trying to allow the sleep command to short circuit

    if( read_temp == last_com + 1){
	last_com++;

	to_return.x = 0;
	to_return.y = 0;
	to_return.x += ( 10 * (buffer[3] - '0'));
	to_return.x += buffer[4] - '0';
	to_return.y += ( 10 * (buffer[5] - '0'));
	to_return.y += buffer[6] - '0';
    }
    else{
	cerr << "Missed communication.(2)" << endl;
	cerr << "Last com: " << last_com << " : " << buffer << endl;
	exit(-1);
    }

    return to_return;
}

void send_move( int& last_sent, int write_filedes, Move move_to_send){
    char buffer[8];
    last_sent++;

    buffer[0] = (last_sent  / 100) + '0';
    buffer[1] = ((last_sent % 100) / 10) + '0';
    buffer[2] = (last_sent % 10) + '0';
    buffer[3] = (move_to_send.x / 10) + '0';
    buffer[4] = (move_to_send.x % 10) + '0';
    buffer[5] = (move_to_send.y / 10) + '0';
    buffer[6] = (move_to_send.y % 10) + '0';
    buffer[7] = '\0';

    write( write_filedes, buffer, sizeof(buffer));
}
