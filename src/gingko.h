/**
 *  gingko.h
 *  gingko
 *
 *  Created on: Mar 9, 2012
 *      Author: auxten
 *
 **/
#ifndef                     _GNU_SOURCE
#define                     _GNU_SOURCE
#endif                      /** _GNU_SOURCE **/

//#define UNITTEST  /* tmp for unittest code write */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <dirent.h>
#include <limits.h>
#include <ftw.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <pthread.h>

#if defined (__APPLE__)
#include <sys/uio.h>
#elif defined(__linux__)
#include <sys/sendfile.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#endif /** __APPLE__ **/

#include "event.h"
#include "gko_errno.h"


#ifndef GINGKO_H_
#define GINGKO_H_


#define                 NO_SO_CALLED_FNV_OPTIMIZE
#define                 ROT_XOR_HASH
#define                 _FILE_OFFSET_BITS       64
/// only 8 or 4 here
#define                 HASH_BYTE_NUM_ONCE      8
#define                 CMD_PREFIX_BYTE         10
#if CMD_PREFIX_BYTE == 10
#define                 PREFIX_CMD              "0000000000"
#endif

/// cmd keyword len
#define                 CMD_LEN                 8

typedef unsigned long long      GKO_UINT64;
typedef long long               GKO_INT64;
typedef const char * const      GKO_CONST_STR;

#if defined (_GKO_VERSION)
static GKO_CONST_STR    GKO_VERSION =           _GKO_VERSION;
#else
static GKO_CONST_STR    GKO_VERSION =           "unknown";
#endif
/// protocol version
static const short      PROTO_VER   =           '0';
/// root uid
static const uid_t      ROOT =                  0;
/// block size in bytes
static const int        BLOCK_SIZE =            (2 * 1024 * 1024);
/// uncompress extra bytes
static const int        UNZIP_EXTRA =           32;// 8 is enough
/// req at max MAX_REQ_SERV_BLOCKS from serv
static const int        MAX_REQ_SERV_BLOCKS =   5;
/// req at max MAX_REQ_CLNT_BLOCKS from clnt
static const int        MAX_REQ_CLNT_BLOCKS =   300;
/// just like ZERO :p
static const int        SERV_PORT =             2120;
/// indicate random port
static const int        RANDOM_PORT =           -1;
/// tcp buffer size
static const int        TCP_BUF_SZ =            262144;
/// request queue length
static const int        REQ_QUE_LEN =           SOMAXCONN;
/// host name length in max
static const int        MAX_HOST_NAME =         255;
/// MAXPATHLEN in linux is 4096!!!,is it too long?
static const u_int      MAX_PATH_LEN =          1024;
/// Lowest port number
static const int        MIN_PORT =              1025;
/// Highest port number
static const int        MAX_PORT =              65535;
/// Maximum listen port
static const int        MAX_LIS_PORT =          60000;
/// Maximum packet length to be received
static const int        MAX_PACKET_LEN =        65536;
/// for global locks
static const int        MAX_JOBS =              1024;
/// max log file keeped
static const u_int      MAX_LOG_KEEPED =        3;
/// max log line count flush log
static const GKO_INT64  MAX_LOG_FLUSH_LINE =    20;
/// max log line count to reopen log file, in case of file mv
static const GKO_INT64  MAX_LOG_REOPEN_LINE =   1000;
/// max log line count
static const GKO_INT64  MAX_LOG_LINE =          (10000 * MAX_LOG_REOPEN_LINE);
/// max log bytes per line
static const GKO_INT64  MAX_LOG_BYTE =          4096;
/// max length of a uri
static const int        MAX_URI =               MAX_PATH_LEN;
/// nftw depth this sames have no effect....whatever 100 is enough
static const int        MAX_SCAN_DIR_FD =       5;
///((1LL<<sizeof(GKO_INT64)*8-1)-1);
static const GKO_INT64  MAX_INT64 =             9223372036854775807LL;
/// retry 3 times then fail
static const int        MAX_DOWN_RETRY =        40;
/// retry 3 times then fail
static const int        SUCC_RETRY_DIV =        2;
/// retry 3 times then fail
static const int        MAX_JOIN_RETRY =        3;
/// retry 3 times then fail
static const int        MAX_HELO_RETRY =        2;
/// min value of ulimit -n
static const GKO_UINT64 MIN_NOFILE =            2000;
/// bind port failed return
static const int        BIND_FAIL =             -12;
/// connect failed return
static const int        HOST_DOWN_FAIL =        -13;
/// connect failed return
static const int        FD_BEFORE_CONNECT =     -1;
/// hello retry interval, in seconds
static const int        HELO_RETRY_INTERVAL =   3;
/// join retry interval, in seconds
static const int        JOIN_RETRY_INTERVAL =   3;
/// bind port try interval, in useconds
static const useconds_t BIND_INTERVAL =         10000;
/// gko.sig_flag check interval, in useconds
static const useconds_t CK_SIG_INTERVAL =       200000;
/// GKO_INT64 int char
static const int        MAX_LONG_INT =          19;
/// default at MacOS is 512k
static const int        MYSTACKSIZE =           (10 * 1024 * 1024);
/// default server conn limit
static const int        SERV_POOL_SIZE =        200000;
/// default client thread num
static const int        SERV_ASYNC_THREAD_NUM = 16;
/// default xor hash thread num
static const int        XOR_HASH_TNUM =         3;
/// host count got from hash ring
static const int        FIRST_HOST_COUNT =      3;
/// host count got from speed test
static const int        SECOND_HOST_COUNT =     3;

/// in seconds
static const int        NET_TIMEOUT =           60;
/// in seconds
static const int        RCV_TIMEOUT =           30;
/// in seconds
static const int        SND_TIMEOUT =           60;

/// sleep time before free the job related mem
static const int        ERASE_JOB_MEM_WAIT =    (SND_TIMEOUT + 5);
///bytes per second
static const int        CLNT_LIMIT_UP_RATE =    (30 * 1024 * 1024);
///bytes per second
static const int        CLNT_LIMIT_DOWN_RATE =  (20 * 1024 * 1024);
///  (disk read rate) / (net upload rate)
static const int        CLNT_DISK_READ_RATIO =  2;
///  (disk write rate) / (net download rate)
static const int        CLNT_DISK_WRITE_RATIO = 2;
///bytes per second
static const int        SERV_LIMIT_UP_RATE =    (95 * 1024 * 1024);
///  (disk read rate) / (net upload rate)
static const int        SERV_DISK_READ_RATIO =  20;
///bytes per second
static const int        LIMIT_MK_SEED_RATE =    (200 * 1024 * 1024);
/// download upload time ratio
static const int        DOWN_UP_TIME_RATIO =    6;
/// max auto seed time
static const int        MAX_AUTO_SEED_TIME =    600;
/// every physical node have VNODE_NUM vnodes
static const int        VNODE_NUM =             3;
/// to sizeof an extern array we must define it
static const int        CMD_COUNT =             9;
/// IP char length
static const int        HOSTNAME_LEN =                256;
/// -rw-rw-rw-
static const mode_t     SNAP_FILE_MODE =        0666;

static const int        SNAP_OPEN_FLAG =        (O_RDONLY | O_CREAT | O_NOCTTY);
static const int        SNAP_REOPEN_FLAG =      (O_WRONLY | O_TRUNC | O_CREAT | O_NOCTTY);
static const int        CREATE_OPEN_FLAG =      (O_WRONLY | O_CREAT | O_NOCTTY);
static const int        READ_OPEN_FLAG =        (O_RDONLY | O_NOCTTY);
static const int        WRITE_OPEN_FLAG =       (O_WRONLY | O_NOCTTY);

static const int        MSG_LEN =               (MAX_URI + HOSTNAME_LEN + 32 + MAX_LONG_INT * 2 + CMD_PREFIX_BYTE);
static const int        CLNT_READ_BUFFER_SIZE = (MSG_LEN + 10);
/// limit read rate when read msg larger than this
static const int        READ_LIMIT_THRESHOLD =  (MSG_LEN + 5);

///"NEWW 255.255.255.255 65535"
static const int        SHORT_MSG_LEN =         (4 + 1 + HOSTNAME_LEN + 1 + 5 + 1 + CMD_PREFIX_BYTE);
/// 10s
static const int        SERV_TRANS_TIME_PLUS =  10000000;
static const int        JOIN_ERR =              1;
static const int        MK_DIR_SYMLINK_ERR =    2;
static const int        DOWNLOAD_ERR =          3;


static GKO_CONST_STR     SERVER_LOG =           "/dev/stdout";
static GKO_CONST_STR     CLIENT_LOG =           "/dev/stdout";
/// file for continue interrupted job
static GKO_CONST_STR     GKO_SNAP_FILE =        "._gk_snapshot_";
/// for debug usage
static GKO_CONST_STR     SERVER_IP =            "127.0.0.1";
/// for log
//static GKO_CONST_STR     TIME_FORMAT =          "[%Y-%m-%d %H:%M:%S]";
static GKO_CONST_STR     TIME_FORMAT =          "[%m-%d %H:%M:%S]";
/// for old log path
static GKO_CONST_STR     OLD_LOG_TIME =         ".%Y%m%d%H%M%S";

/// host_hash usage
static const u_int8_t  DEL_HOST =                 0x01;
static const u_int8_t  ADD_HOST =                 0x02;

/// job state
static const u_int8_t  INITING =                  0x01;    ///initializing
static const u_int8_t  INITED =                   0x02;    ///initialized

/// lock state
static const u_int8_t  LK_USING =                 0x01;
static const u_int8_t  LK_FREE =                  0x00;

/// get_blocks_c flag
static const u_int8_t  W_DISK =                   0x01;


/// calculate the offset of a member in struct
#define OFFSETOF(s, i) ((unsigned) &((s *)0)->i)

/// Macros for min/max.
#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif /** MIN **/
#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif  /** MAX **/

/// calculate block count by size
#define BLOCK_COUNT(a) \
    (((a) % (BLOCK_SIZE)) ? ((a)/(BLOCK_SIZE) + 1) : ((a)/(BLOCK_SIZE)))

/// reload > for s_host_t
#ifndef GINGKO_OVERLOAD_S_HOST_LT
#define GINGKO_OVERLOAD_S_HOST_LT \
static bool operator <(const s_host_t& lhost, const s_host_t& rhost) {\
    return (((((GKO_UINT64) ntohl(inet_addr(lhost.addr))) << 16)\
            + (GKO_UINT64) lhost.port) < ((((GKO_UINT64) ntohl(\
            inet_addr(rhost.addr))) << 16) + (GKO_UINT64) rhost.port));\
}
#endif /** GINGKO_OVERLOAD_S_HOST_LT **/

/// reload == for s_host_t
#ifndef GINGKO_OVERLOAD_S_HOST_EQ
#define GINGKO_OVERLOAD_S_HOST_EQ \
static bool operator ==(const s_host_t& lhost, const s_host_t& rhost) {\
    return ( strncmp(lhost.addr, rhost.addr, sizeof(lhost.addr)) == 0 &&\
        lhost.port == rhost.port);\
}
#endif /** GINGKO_OVERLOAD_S_HOST_EQ **/

/// Macro judging errno
#define ERR_RW_RETRIABLE(e) \
    ((e) == EINTR || (e) == EAGAIN || (e) == EWOULDBLOCK)
#define CONNECT_DEST_DOWN(e) \
    ((e) == ECONNREFUSED || (e) == ENETUNREACH || (e) == ETIMEDOUT)

/// Evaluates to the same boolean value as 'p', and hints to the compiler that
/// we expect this value to be false.
#ifdef __GNUC__
#define LIKELY(p) __builtin_expect(!!(p), 1)
#define UNLIKELY(p) __builtin_expect(!!(p), 0)
#else
#define LIKELY(p) (p)
#define UNLIKELY(p) (p)
#endif
#define SUCC_CHECK(cond) LIKELY(cond)
#define FAIL_CHECK(cond) UNLIKELY(cond)

/// Macro for unittest
#ifndef UNITTEST
#define GKO_STATIC_FUNC static
#else
#define GKO_STATIC_FUNC
#endif

/// Replacement for assert() that log to file on failure.
#define ASSERT(cond)                     \
    do {                                \
        if (UNLIKELY(!(cond))) {             \
            GKOLOG(FATAL, "Assertion: (%s) failed, %s() in %s:%d :",     \
                 #cond,__func__,__FILE__,__LINE__);      \
            /** In case a user-supplied handler tries to **/  \
            /** return control to us, log and abort here. **/ \
            (void)fprintf(stderr,               \
                "Assertion: (%s) failed, %s() in %s:%d \n",     \
                #cond,__func__,__FILE__,__LINE__);      \
            exit(-11);                    \
        }                           \
    } while (0)

#define LOG_IF_NOT(cond)                     \
    do {                                \
        if (UNLIKELY(!(cond))) {             \
            GKOLOG(WARNING, "Assertion: (%s) failed, %s() in %s:%d : ",     \
                 #cond,__func__,__FILE__,__LINE__);      \
            /** In case a user-supplied handler tries to **/  \
            /** return control to us, log and abort here. **/ \
            (void)fprintf(stderr,               \
                "Assertion: (%s) failed, %s() in %s:%d ",     \
                #cond,__func__,__FILE__,__LINE__);                  \
        }                           \
    } while (0)


/// some struct define
#pragma pack (4)

typedef struct _s_job_t s_job_t;
typedef void * (*func_t)(void *, int);
typedef void * (*ProcessHandler_t)(void *);
typedef void (*ReportHandler_t)(void *, const char *);

/**
 * @brief How Many Bytes to Read
 *      set "How Many To Read" handler
 *      the handler prototype is as:
 *          int (*HMTRHandler_t)(void *, const char *, const int);
 *      @ret:
 *          n == 0  --> need read more
 *          n > 0   --> need read n byte(s) in total, NOT n byte(s) more!!
 *          n < 0   --> an error in the message
 *      @args:
 *          void *          --> <struct conn_client *> for the specified connection
 *          const char *    --> the data already read
 *          const int       --> the data count already read, in byte
 *
 *
 * @see
 * @note
 * @author auxten  <auxtenwpc@gmail.com>
 * @date 2012-9-29
 **/
typedef int (*HMTRHandler_t)(void *, const char *, const int);

/// file structure
typedef struct _s_file_t
{
    struct stat f_stat;
    mode_t mode;
    GKO_INT64 size; /// -1 for dir, -2 for symbol link
    char sympath[MAX_PATH_LEN];
    char name[MAX_PATH_LEN];
    u_int8_t md5[16];
} s_file_t;

/// host structure
typedef struct _s_host_t
{
    char addr[HOSTNAME_LEN];
    int port;
} s_host_t;

/// block structure
typedef struct _s_block_t
{
    GKO_INT64 size;
    GKO_INT64 start_off;
    GKO_INT64 start_f;
    u_int8_t done;
    unsigned int digest;
    std::set<s_host_t> * host_set; ///only used by client, lock here
} s_block_t;

/// arguments for thread
typedef struct
{
    GKO_INT64 range[2];
    s_job_t * p;
    u_int8_t * buf;
    int index;
} hash_worker_thread_arg;

/// job structure
typedef struct _s_job_t
{
    unsigned int job_state;    /// pass job state to client
    int host_num;
    int host_set_max_size;  /// keep track of the max host_set size
    int lock_id;
    struct timeval dl_time; /// time used for download
    char uri[MAX_URI];
    char path[MAX_PATH_LEN]; ///localpath, serv part and clnt part are diff
    s_file_t * files;
    GKO_INT64 files_size;
    s_block_t * blocks;
    GKO_INT64 blocks_size;
    std::set<s_host_t> * host_set; ///lock here
    GKO_INT64 file_count;
    GKO_INT64 block_count;
    GKO_INT64 total_size;
    GKO_INT64 hash_progress[XOR_HASH_TNUM]; /// the hash progress in byte
    /** hash worker threads **/
    pthread_t hash_worker[XOR_HASH_TNUM];
    u_int8_t * hash_buf[XOR_HASH_TNUM];
    hash_worker_thread_arg arg[XOR_HASH_TNUM];
} s_job_t;

/// store host hash result
typedef struct _s_host_hash_result_t
{
    GKO_INT64 v_node[VNODE_NUM];
    GKO_INT64 length[VNODE_NUM];
} s_host_hash_result_t;

/// for seed TLS usage
typedef struct _s_seed_t
{
    GKO_INT64 file_count;
    GKO_INT64 init_s_file_t_iter;
    s_file_t * files;
    s_block_t * blocks;
    GKO_INT64 init_s_block_t_iter;
    GKO_INT64 total_size;
    GKO_INT64 tmp_size;
    GKO_INT64 last_init_block;
} s_seed_t;

/// job specified lock
typedef struct _s_lock_t
{
    char state;
    pthread_mutex_t lock;
} s_lock_t;

/// for passing write arg in libevent callback
typedef struct _s_write_arg_t
{
    int sent;
    int send_counter;
    int flag;
    int retry;
    int sz;
    char * p;
    struct event_base *ev_base;
    struct event ev_write;
} s_write_arg_t;

/// for passing sendfile arg in libevent callback
typedef struct _s_gsendfile_arg_t
{
    GKO_INT64 sent;
    GKO_UINT64 send_counter;
    int in_fd;
    int retry;
    off_t offset;
    GKO_UINT64 count;
    struct event_base *ev_base;
    struct event ev_write;
} s_gsendfile_arg_t;

/// vnode download params
typedef struct _s_vnode_download_arg_t
{
    s_job_t * jo;
    GKO_INT64 blk_idx;
    GKO_INT64 blk_count;
} s_vnode_download_arg_t;

/// async server params
typedef struct _s_async_server_arg_t
{
    s_host_t * s_host_p;
} s_async_server_arg_t;

/// for args processing
typedef struct _s_option_t
{
    char need_help;
    char to_continue;
    char need_progress;
    int to_debug;
    int port;
    int worker_thread;
    int connlimit;
    int max_dir_depth;
    int seed_time;
    /// up band width limit rate
    int limit_up_rate;
    /// down band width limit rate
    int limit_down_rate;
    /// read disk limit rate
    int limit_disk_r_rate;
    /// write disk limit rate
    int limit_disk_w_rate;
    /// make seed speed limit rate
    int limit_mk_seed_rate;
    in_addr_t bind_ip; /// this is usually 32bit
    char logpath[MAX_PATH_LEN];
} s_option_t;

/// gingko global stuff
typedef struct _s_gingko_global_t
{
    FILE * log_fp;

    /// options switch
    s_option_t opt;

    ///  the host and server
    s_host_t the_serv;

    /// ready_to_serv flog, when the upload thread
    volatile char ready_to_serv;

    /// save the NEWWed, DELEed host when server is not ready
    std::vector<s_host_t> hosts_new_noready;
    std::vector<s_host_t> hosts_del_noready;

    /// snap file path
    char snap_fpath[MAX_PATH_LEN];
    int snap_fd;

    /// signal flag
    volatile u_int8_t sig_flag;
} s_gingko_global_t;

/// for snap file read and write
typedef struct _s_snap_t
{
    unsigned int digest;
    u_int8_t done;
} s_snap_t;
#pragma pack ()


/************** FUNC DICT **************/
/// send HELO handler
int helo_serv_c(void * arg, int fd, s_host_t * server);
/// send JOIN handler
void * join_job_c(void *, int);
/// send GETT handler
GKO_INT64 get_blocks_c(s_job_t * jo, s_host_t * dhost, GKO_INT64 num, GKO_INT64 count,
        u_int8_t flag, char * buf);

/************** FUNC DECL **************/
/// send JOIN handler
void abort_handler(const int sig);
/// send JOIN handler
void set_sig(void(*int_handler)(int));
/// start a thread to watch the gko.sig_flag and take action
int sig_watcher(void * (*worker)(void *));
void int_handler(const int sig);
void * int_worker(void * a);

/// send a mem to fd(usually socket)
int sendall(int, const void *, int sz, int flag);
/// send a mem to fd(usually socket)
int sep_arg(char * inputstring, char * arg_array[], int max);
/// exit wrapper
void gko_quit(int ret);
/// parse the request return the proper func handle num
int parse_req(char *req);
/// parse the request return the proper func handle num
struct hostent * gethostname_my(const char *host, struct hostent *hostbuf);
/// get host in_addr_t, this only works on IPv4. that is enough
int getaddr_my(const char *host, in_addr_t * addr_p);
/// check ulimit -n
int check_ulimit();
///// erase job related stuff
//int erase_job(string &uri_string);
/// hash the host to the data ring
s_host_hash_result_t * host_hash(s_job_t * jo, const s_host_t * new_host,
        s_host_hash_result_t * result, const u_int8_t usage);
/// send blocks to the out_fd(usually socket)
int sendblocks(int out_fd, s_job_t * jo, GKO_INT64 start, GKO_INT64 num);
/// send zipped blocks to the out_fd(usually socket)
int sendblocks_zip(int out_fd, s_job_t * jo, GKO_INT64 start, GKO_INT64 num);
/// write block to disk
int writeblock(s_job_t * jo, const u_int8_t * buf, s_block_t * blk);
/// send cmd msg to host, not read response, on succ return 0
int sendcmd2host(const s_host_t *h, const char * cmd, const int recv_sec, const int send_sec);
/// send cmd msg to host, read response, on succ return 0
int chat_with_host(const s_host_t *h, const char * cmd, const int recv_sec, const int send_sec);
/// try best send cmd to *fd, with retry twice then get response
int chat_fd(const char * host, const int port, int * fd, const char * cmd, const int cmd_len, char * response, const int max_response, const int timeout);
/// try best send cmd to *fd, with reconnect
int send2host_fd(const char * host, const int port, int * fd, const char * cmd, const int cmd_len, const int timeout);
/// try best to read specificed bytes from a file to buf
int readfileall(int fd, off_t offset, off_t count, char ** buf);
/// try best to read specificed bytes from a file to buf
int readall(int socket, void* data, int data_len, int flags);
/// try best to read cmd, the first 2 bytes are cmd length
int readcmd(int fd, void* data, int max_len, int timeout);
/// update host_set_max_size
void update_host_max(s_job_t * job);

/************** INLINE FUNC DECL **************/
/**
 * @brief calculate the sum of an given length array, return the sum
 *
 * @see
 * @note
 * @author auxten  <auxtenwpc@gmail.com>
 * @date 2011-8-1
 **/
static inline GKO_INT64 array_sum(GKO_INT64 * a, GKO_INT64 count)
{
    GKO_INT64 sum = 0;
    while (count--)
    {
        sum += *(a + count);
    }
    return sum;
}

/**
 * @brief return index num of the next file in the s_file_t array
 *
 * @see
 * @note
 * @author auxten  <auxtenwpc@gmail.com>
 * @date 2011-8-1
 **/
static inline GKO_INT64 next_f(s_job_t * jo, GKO_INT64 file)
{
    do
    {
        file = (1 + file) % (jo->file_count);
    } while (((jo->files) + file)->size <= 0);
    return file;
}

/**
 * @brief return index num of the next block in the s_block_t array
 *        max + 1 = 0
 *
 * @see
 * @note
 * @author auxten  <auxtenwpc@gmail.com>
 * @date 2011-8-1
 **/
static inline GKO_INT64 next_b(s_job_t * jo, GKO_INT64 block)
{
    return (++block) % (jo->block_count);
}

/**
 * @brief return index num of the previous block in the s_block_t array
 *        0 - 1 = max
 *
 * @see
 * @note
 * @author auxten  <auxtenwpc@gmail.com>
 * @date 2011-8-1
 **/
static inline GKO_INT64 prev_b(s_job_t * jo, GKO_INT64 block)
{
    return (block ? block - 1 : jo->block_count - 1);
}

/**
 * @brief calculate the s_host_t ip distance
 *
 * @see
 * @note
 * @author auxten  <auxtenwpc@gmail.com>
 * @date 2011-8-1
 **/
static inline GKO_UINT64 host_distance(const s_host_t * h1, const s_host_t * h2)
{
    return ((((GKO_UINT64) ntohl(inet_addr(h1->addr))) << 16)
            + (GKO_UINT64) h1->port)
            ^ ((((GKO_UINT64) ntohl(inet_addr(h2->addr))) << 16)
                    + (GKO_UINT64) h2->port);
}

/**
 * @brief get thread id, Darwin has no thread idp
 *
 * @see
 * @note
 * @author auxten  <auxtenwpc@gmail.com>
 * @date 2011-8-1
 **/
static inline pid_t gko_gettid(void)
{
#if defined (__APPLE__)
    return getpid();
#elif defined(__linux__)
    return syscall(SYS_gettid);
#endif
}

/**
 * @brief sendfile wrapper
 *
 * @see
 * @note
 * @author auxten  <auxtenwpc@gmail.com>
 * @date 2011-8-1
 **/
static inline int gsendfile(int out_fd, int in_fd, off_t *offset,
        GKO_UINT64 *count)
{
#if defined (__APPLE__)
    int ret = sendfile(in_fd, out_fd, *offset, (off_t *) count, NULL, 0);
    if (ret == -1 && !ERR_RW_RETRIABLE(errno))
        return (-1);

    return (*count);
#elif defined (__FreeBSD__)
    int ret = sendfile(in_fd, out_fd, *offset, *count, NULL, (off_t *) count, 0);
    if (ret == -1 && !ERR_RW_RETRIABLE(errno))
        return (-1);

    return (*count);
#elif defined(__linux__)
    int ret = sendfile(out_fd, in_fd, offset, *count);
    if (ret == -1 && ERR_RW_RETRIABLE(errno))
    {
        /** if this is EAGAIN or EINTR return 0; otherwise, -1 **/
        return (0);
    }
    return (ret);
#endif
}

/**
 * @brief fill the cmd header before send it out, including version, cmd length
 *
 * @see
 * @note
 * @author auxten  <auxtenwpc@gmail.com>
 * @date Jan 10, 2012
 **/
static inline void fill_cmd_head(char * cmd, int msg_len)
//static void fill_cmd_head(char * cmd, int msg_len)
{
    memset(cmd, '0', CMD_PREFIX_BYTE);

    char * p = cmd + CMD_PREFIX_BYTE - 1;
    while (msg_len > 0)
    {
        *(p--) = (char)(msg_len % 10) + '0';
        msg_len /= 10;
    }
}

static inline void parse_cmd_head(const char * cmd, void * proto_ver, int * msg_len)
{
    if (proto_ver)
        * (unsigned short *)proto_ver = 0;
    *msg_len = 0;
    const char * p = cmd;
    while (p - cmd < CMD_PREFIX_BYTE)
    {
        if (*p > '9' || *p < '0')
        {
            *msg_len = -1;
            break;
        }
        *msg_len = (*msg_len) * 10 + *(p++) - '0';
    }
}

#endif /** GINGKO_H_ **/
