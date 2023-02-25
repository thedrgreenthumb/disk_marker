#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


void usage(void)
{
	printf("Add header with offset and block number to every sector of provided file/device\n");
	printf("Usage example: ./marker /path/to/file 512 10\n");
	printf("Where 512 is sector size\n");
	printf("Where 10 sectors count to process\n");
	printf("WARNING: Do not use with real files/drvies with sensible data!\n");

	exit(0);
}

int main(int argc, char *argv[])
{
	char *path;
	char *buf = NULL;
	int sector_size;
	int sectors = 0;
	int count;
	off_t offset = 0;
	int fd;
	int ret;

	if (argc != 4)
		usage();

	path = argv[1];
	sector_size = atoi(argv[2]);
	if (sector_size < 512) {
		printf("Bad sector size %d\n", sector_size);
		exit(1);
	}

	count = atoi(argv[3]);
	if (count < 1) {
                printf("Bad sectors count %d\n", count);
                exit(1);
        }

	fd = open(path, O_RDWR);
	if (fd < 0) {
		printf("Cannot open %s, err=%d\n", path, errno);
		exit(1);
	}

	do {
		if (sectors % (count/10) == 0)
			printf("%d sectors processed...\n", sectors);

		free(buf);
		buf = calloc(1, sector_size);
		sprintf(buf, "%lx -> %x", offset, sectors);
		sectors++;
		offset+=sector_size;
	} while ((ret = write(fd, buf, sector_size)) == sector_size && sectors < count);

	return 0;
}
