/*
 * This sample code shows how to use Libevent to manage various events in 
 * linux user space process, including following very typical I/O :
 *  - Event from a named pipe file (this can be extended to device file).
 *  - Event from a TCP socket
 *  - Event from a timer expiration
 *  - Event from a signal received
 *  - Event from a file operation using inotify (file create, delete, modification)
 *
 * The nice thing about this event driven framework is all events are processed sequentially
 * so that the programming model is simpler (no concurrency), hopefully make it more robust code.
 *
 * This has been tested in ubuntu 16.04 using libevent version 2.1.11
 *
 * Build command:
 *
 * gcc -I/usr/local/include  -o events-mgmnt events-mgmnt.c -L/usr/local/lib -levent
 *
 * To execute, set below:
 * export LD_LIBRARY_PATH=/usr/local/lib
 *
 * Author Wenwei Weng
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/inotify.h>

#include <event2/event.h>
#include <event2/event-config.h>
#include <event2/event_struct.h>
#include <event2/util.h>

const char *fifo = "/tmp/event.fifo";
static int file_fd = -1;

/* 
 * Callback function for file read event handling.
 */
static void file_read_cb(evutil_socket_t fd, short event, void *arg)
{
    char buf[255];
    int len;
    struct event *ev = arg;

    if (fd != file_fd) {
	
    }
    
    fprintf(stderr, "file_read_cb called with fd: %d, event: %d, arg: %p\n",
	    (int)fd, event, arg);

    len = read(fd, buf, sizeof(buf) - 1);

    if (len <= 0) {
	if (len == -1) {
	    perror("read");
	    return;
	} else if (len == 0) {
	    fprintf(stderr, "Nothing to read ? or Connection closed ?\n");
	    event_del(ev);
	    event_base_loopbreak(event_get_base(ev));
	    return;
	}
    }

    buf[len] = '\0';

    fprintf(stdout, "Read: %s\n", buf);

}

/*
 * Create a FIFO read event to be enrolled into main loop. 
 */
static struct event *create_file_read_event(struct event_base* base)
{
    struct event *evfile;
    
    // create fifo file to receive msg...
    unlink(fifo);
    if (mkfifo(fifo, 0600) == -1) {
	perror("mkfifo");
	exit(1);
    }

    // pipe file if open only with O_RDONLY, it will fire infinity read event
    // because with O_RDONLY mode, it always make read ready for named pipe file
    // workaround is to change to open as read and write both, i.e. O_RDWR
    file_fd = open(fifo, O_RDWR | O_NONBLOCK, 0);
    if (file_fd == -1) {
	perror("open fifo file failed");
	exit(1);
    }

    // add fd as EV_PERSIST meaning it will keep waiting for next new input
    evfile = event_new(base, file_fd, (EV_READ | EV_PERSIST), file_read_cb,
		       event_self_cbarg());

    return evfile;
}

struct signal_evt_ctx {
    int sig_val;
    void *event;
};

struct signal_evt_ctx signal_ctx[SIGSYS+1];
static int sig_cxt_init = 0;

/*
 * Call back function for signal handling
 */
static void signal_cb(evutil_socket_t fd, short event, void *arg)
{
    struct event_base *base = (struct event_base*)((struct signal_evt_ctx *)arg)->event;

    if (((struct signal_evt_ctx *)arg)->sig_val == SIGINT) {
	fprintf(stdout, "\n SIGINT received, exit, bye! \n");
	event_base_loopbreak(base);
    } else {
	fprintf(stdout, "\n Received signal is %d \n ",
		((struct signal_evt_ctx *)arg)->sig_val); 
    }
}

/*
 * create event for the given signal to be enrolled.
 */
static struct event* create_signal_event(struct event_base* base, int signal)
{
    // initialize signal context for all signals once
    if (!sig_cxt_init) {
	for (int i = 0; i <= SIGSYS; i++)
	    signal_ctx[i].sig_val = i;
	sig_cxt_init = 1;
    }
    struct signal_evt_ctx * sig_ctx = &signal_ctx[signal];
    sig_ctx->event = base;
    return evsignal_new(base, signal, signal_cb, sig_ctx);
}

struct timeval lasttime;
static int timer_interval = 5; //seconds

/*
 * Call back function for timer exp event handling
 */
static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    struct timeval newtime, difference;
    struct event *timeout = arg;
    double elapsed;

    evutil_gettimeofday(&newtime, NULL);
    evutil_timersub(&newtime, &lasttime, &difference);
    elapsed = difference.tv_sec + (difference.tv_usec / 1.0e6);

    printf("timeout_cb called at %d: %.3f seconds elapsed.\n",
	   (int)newtime.tv_sec, elapsed);
    lasttime = newtime;

#if 0
    // re-arm the timer if needed on the fly for non recurring timer
    // this is just a note, N/A here
    int rearm_timer_ifneeded = call_foo_rearm_timer_required();
    if (rearm_timer_ifneeded) {
	struct timeval tv;
	evutil_timerclear(&tv);
	tv.tv_sec = 2;
	event_add(timeout, &tv);
    }
#endif
}

/*
 * Create event for a timer with given interval time.
 */
static void create_timeout_event(struct event_base* base, struct event *timeout,
				 struct timeval *tv, int recurring_timer)
{
    int flags=  recurring_timer ? EV_PERSIST : 0;

    // fill in timeout event
    event_assign(timeout, base, -1 /*FD*/, flags, timeout_cb, (void*)timeout);

    // set time
    evutil_timerclear(tv);
    tv->tv_sec = timer_interval;

    evutil_gettimeofday(&lasttime, NULL);
}

/* 
 * Open a TCP server socket on given port
 */
static int sock_fd = -1;
static int create_socket_fd(int port) {
    struct sockaddr_in serv_addr; 

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
	perror("open socket failed");
	exit(1);
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);    
    serv_addr.sin_port = htons(port); 
    bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(sock_fd, 10);
    
    return sock_fd;
}

/*
 * Call back function for timer exp event handling
 */
static void socket_cb(evutil_socket_t fd, short event, void *arg) {
    fprintf(stdout, "\n handling socket event: accept and close it! \n");

    close(accept(fd, (struct sockaddr*)NULL, NULL));
}

/*
 * Create event for socket event handling
 */
static struct event* create_socket_event(struct event_base* base, int port)
{
    sock_fd = create_socket_fd(port);
    return event_new(base, sock_fd, EV_READ | EV_PERSIST, socket_cb,
		     event_self_cbarg());
}

static int inotify_fd = -1;
static int wd = -1;
const char* dir_watch = "/tmp/my-prog/";

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
/*
 * Call back function for file operation from inotify event handling
 * Note: inotify doesn't seem working well on file, the workaround is 
 * to watch directory, if there is a new file created, it is detected,
 * if it is removed, it is detected as well, but as a directory modified.
 * if the file content (e.g. /tmp/my-prog/test) is modified, it is detected as
 * modify event as well.
 */
static void inotify_cb(evutil_socket_t fd, short event, void *arg) {
    char buffer[BUF_LEN];
    int i = 0, event_msg_len = 0;

    fprintf(stdout, "\n handling inotify event: \n");

    memset(buffer, 0, BUF_LEN);
    event_msg_len = read( fd, buffer, BUF_LEN );
    if (event_msg_len < 0) {
	perror( "read" );
	return;
    }

    while (i < event_msg_len) {
	struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
	fprintf(stdout, "\n          inotify event mask:0x%x: \n", event->mask);
	if (event->len) {
	    if ( event->mask & IN_CREATE ) {
		printf( "The file %s was created.\n", event->name );
	    } else if ((event->mask & IN_DELETE) || (event->mask & IN_DELETE_SELF)) {
		printf( "The file %s was deleted.\n", event->name );
	    } else if ( event->mask & IN_MODIFY ) {
		printf( "The file %s was modified.\n", event->name );
	    }
	}
	i += EVENT_SIZE + event->len;
    }
}

/*
 * Create event for inotify event handling
 */
static struct event* create_inotify_event(struct event_base* base)
{
    inotify_fd = inotify_init();

    if (inotify_fd < 0) {
	perror("open inotify failed");
	exit(1);
    }
    wd = inotify_add_watch(inotify_fd, dir_watch,
			   IN_MODIFY | IN_CREATE | IN_DELETE );
    
    return event_new(base, inotify_fd, EV_READ | EV_WRITE | EV_PERSIST, inotify_cb,
		     event_self_cbarg());
}

/*
 * Any necessary cleanup after exit from main event loop
 */
static void local_cleanup_needed()
{
    unlink(fifo);
    close(file_fd);
    close(sock_fd);
    close(inotify_fd);
}

int main(int argc, char **argv)
{
    struct event_base* base;
    struct event *evfile, *evsocket, *evsig_int, *evsig_usr1, *evinotify;
    struct event evtimeout;
    struct timeval tv;

    // Initalize the event library 
    base = event_base_new();

    //////////////////////////////////////////////////////////
    // file FD handling example using named pipe as example
    // this is applicable to device file such as /dev/ttyS1
    evfile = create_file_read_event(base);
    /* Add it to the active events, without a timeout */
    event_add(evfile, NULL);

    ////////////////////////////////////////////////////////
    // signal handling example
    
    /* catch SIGINT so that we can quit event processing and clean up */
    /* below API call will create signal FD and add into watch list*/
    evsig_int = create_signal_event(base, SIGINT);
    event_add(evsig_int, NULL);

    evsig_usr1 = create_signal_event(base, SIGUSR1);
    event_add(evsig_usr1, NULL);

    ///////////////////////////////////////////////////////
    // timer handling example
    create_timeout_event(base, &evtimeout, &tv, 1 /*recurring */); 
    event_add(&evtimeout, &tv);

    ///////////////////////////////////////////////////
    // socket handling example
    evsocket = create_socket_event(base, 5678);
    // Add it to the active events, without a timeout 
    event_add(evsocket, NULL);

    ////////////////////////////////////////////////////
    // file operation event from inotify
    evinotify = create_inotify_event(base);
    event_add(evinotify, NULL);
   
    ////////////////////////////////////////////////////
    // ready to start main event loop ...
    event_base_dispatch(base);

    /////////////////////////////////////////////////////
    // somehow we were told to exit main event loop, cleaning up
    event_base_free(base);

    local_cleanup_needed();
    
    libevent_global_shutdown();
    
    return (0);
}

