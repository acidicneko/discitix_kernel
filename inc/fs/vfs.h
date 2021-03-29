#ifndef _VFS_H_
#define _VFS_H_

#include <stdint.h>
#include <stddef.h>

/* NOTE!!   
This VFS Model is from JamesMolloy's tutorial! This will be changes later1
I am making this to learn VFS and later implement my own!
*/

typedef struct{
    char name[128]; // This should not contain name according to POSIX
    uint32_t flags; // don't know if this is right -_-
    uint32_t mask;
    uint32_t uid;
    uint32_t gid;
    uint32_t flags;
    uint32_t inode;
    uint32_t length;
    uint32_t impl; // wtf is this??
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir;
    fs_node_t *ptr; // ???????
} fs_node_t;

typedef struct{
    char name[128]; // what the hell is going on??????
    uint32_t inode_number;
}

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08

typedef uint32_t (*read_type_t)(fs_node_t*, uint32_t, uint32_t, uint8_t*);
typedef uint32_t (*write_type_t)(fs_node_t*, uint32_t, uint32_t, uint8_t*);
typedef void (*open_type_t)(fs_node_t*);
typedef void (*close_type_t)(fs_node_t*);
typedef struct dirent * (*readdir_type_t)(fs_node_t*, uint32_t);
typedef fs_node_t * (*finddir_type_t)(fs_node_t*, char *name); 

extern fs_node_t *fs_root;

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void open_fs(fs_node_t *node, uint8_t read, uint8_t write);
void close_fs(fs_node_t *node);
struct dirent *readdir_fs(fs_node_t *node, uint32_t index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);