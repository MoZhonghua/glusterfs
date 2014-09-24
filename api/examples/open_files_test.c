#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "api/glfs.h"
#include "api/glfs-handles.h"

void read_urandom(void* buf, size_t n) {
	int count = 0;
	FILE* fp = NULL;
	fp = fopen("/dev/urandom", "rb");
	if (!fp) {
		fprintf(stderr, "failed to open /dev/urandom: %s", strerror(errno));
		exit(-1); 
	}

	while (n > 0) {
		count = fread(buf, 1, n, fp);
		if (count < 0) {
			fprintf(stderr, "fread(dev/urandom): %s", strerror(errno));
			exit(1);
		}
		n -= count;
		buf += count;
	}
	fclose(fp);
}

/* /c1/5b/c15b1faeb3614fdf88b1789bf8ba39d8 */
void generate_rand_file_name(char* name) {
	unsigned char buf[128];
	int i = 0;

	read_urandom(buf, 128);
	*(name++) = '/';
	snprintf(name, 3, "%02x", (int)buf[0]);
	name += 2;
	*(name++) = '/';
	snprintf(name, 3, "%02x", (int)buf[1]);
	name += 2;
	*(name++) = '/';
	for (i = 0; i < 16; ++i) {
		snprintf(name, 3, "%02x", (int)buf[i]);
		name += 2;
	}
	*(name++) = '\0';
}

void test_open_existing_file_for_append(glfs_t* fs, const char* filename) {
	char buf[512];
	glfs_fd_t* fd = NULL;
	int ret = 0;

	fd = glfs_creat(fs, filename, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (!fd) {
		fprintf(stderr, "open - %s: (%p) %s\n", filename, fd, strerror(errno));
		exit(0);
	}
	ret = glfs_write(fd, buf, 512, 0);
	if (ret != 512) {
		fprintf(stderr, "write - %s: (%p) %s\n", filename, fd, strerror(errno));
		exit(0);
	}
	glfs_close(fd);
}

void test_open_nonexisting_file_for_append(glfs_t* fs, const char* filename) {
	char buf[512];
	glfs_fd_t* fd = NULL;
	int ret = 0;

	// O_CREAT not work for glfs_open
	fd = glfs_open(fs, filename, O_CREAT | O_APPEND | O_WRONLY);
	assert (fd == 0);
	fd = glfs_open(fs, filename, O_CREAT | O_WRONLY);
	assert (fd == 0);

	fd = glfs_creat(fs, filename, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (!fd) {
		fprintf(stderr, "open - %s: (%p) %s\n", filename, fd, strerror(errno));
		exit(0);
	}
	ret = glfs_write(fd, buf, 512, 0);
	if (ret != 512) {
		fprintf(stderr, "write - %s: (%p) %s\n", filename, fd, strerror(errno));
		exit(0);
	}
	glfs_close(fd);
}

void test_stat_file(glfs_t* fs, const char* path) {
	struct stat st = {0};
	int ret = 0;

	ret = glfs_stat(fs, path, &st);
	if (ret != 0) {
		fprintf(stderr, "stat - %s: %s\n", path, strerror(errno));
		exit(0);
	}

	printf("size: %ld\n", st.st_size);
}

int main() 
{
	glfs_t* fs;
	glfs_fd_t* fd = NULL;
	// char buf[512];
	int ret = 0;
	char filename[64];

	for (;;) {
		printf("==============================\n");
		fs = glfs_new("afrvol");
		if (!fs) {
			fprintf(stderr, "glfs_new: returned NULL\n");
			return 1;
		}

		ret = glfs_set_volfile_server(fs, "tcp", "x2", 24007);
		/* 7: debug
		 * 3: error 
		 */
		ret = glfs_set_logging(fs, "/dev/stderr", 3); 
		ret = glfs_init(fs);
		fprintf (stderr, "glfs_init: returned %d\n", ret);

		sleep(2);
		for (;;) {
			generate_rand_file_name(filename);
			/* printf("creating %s\n", filename); */
			printf("%s\n", filename);
			test_open_nonexisting_file_for_append(fs, filename);
			test_stat_file(fs, filename);
			test_open_existing_file_for_append(fs, filename);
			test_stat_file(fs, filename);
			break;
		}

		// out:
		if (fd) {
			glfs_close(fd);
			fd = NULL;
		}

		if (fs) {
			glfs_fini(fs);
			fs = NULL;
		}
	}

	return 0;
}

