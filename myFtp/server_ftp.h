#include <commons.h>
#include <file_transfer_functions.h>

#include <time.h>
#include <sys/stat.h>
#define	ID	"SERVER=> "

struct client_info
{
	int sfd;
	int cid;
};

struct client_info* client_info_alloc(int, int);

void command_cd(struct packet*, struct packet*, int, char*);
void command_ls(struct packet*, struct packet*, int, char*);
void command_get(struct packet*, struct packet*, int);
void command_put(struct packet*, struct packet*, int);

