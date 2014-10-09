#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "api/glfs.h"
#include "api/glfs-handles.h"

enum {
	GLFS_API = 0,
	MOUNT,
};

int64_t now_us() {
	const int64_t kMicrosecondsPerSecond = 1000 * 1000;
	int64_t us = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	us = tv.tv_sec * kMicrosecondsPerSecond + tv.tv_usec;
	return us;
}

void now_string(char *buf) {
	time_t t = time(0);
	strftime(buf, 9, "%H:%M:%S", localtime(&t));
}

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

typedef struct {
	char name[40];
} name_t;

/* /c1/5b/c15b1faeb3614fdf88b1789bf8ba39d8 */
void generate_rand_file_name(char* name) {
	unsigned char buf[128];
	int i = 0;
	char *out = name;

	read_urandom(buf, 128);
	*(out++) = '/';
	snprintf(out, 3, "%02x", (int)buf[0]);
	out += 2;
	*(out++) = '/';
	snprintf(out, 3, "%02x", (int)buf[1]);
	out += 2;
	*(out++) = '/';
	for (i = 0; i < 16; ++i) {
		snprintf(out, 3, "%02x", (int)buf[i]);
		out += 2;
	}
	*(out++) = '\0';
	assert (out-name == 40);
}

void test_open_existing_file_for_append(glfs_t* fs, const name_t* name) {
	char buf[512];
	glfs_fd_t* fd = NULL;
	int ret = 0;

	fd = glfs_creat(fs, name->name, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (!fd) {
		fprintf(stderr, "open - %s: (%p) %s\n", name->name, fd, strerror(errno));
		exit(0);
	}
	ret = glfs_write(fd, buf, 512, 0);
	if (ret != 512) {
		fprintf(stderr, "write - %s: (%p) %s\n", name->name, fd, strerror(errno));
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


struct api_write_data {
	int type;
	const char* path;
	glfs_t* fs;
	size_t file_size;
	int file_count;
	int worker_no;
};

void api_write_files(struct api_write_data* d) 
{
	int i = 0;
	int ret = 0;
	char file_name[128];
	char* data;
	int64_t start = 0;
	int64_t now = 0;
	int64_t creat_time = 0;
	int64_t write_time = 0;
	int64_t close_time = 0;
	int64_t one_file_total_time = 0;
	int64_t total_time = 0;
	int success = 0;
	int failure = 0;
	glfs_fd_t* fd = NULL;

	data = (char*)malloc(d->file_size);
	read_urandom(data, d->file_size);
	for (; i < d->file_count; ++i) {
		generate_rand_file_name(file_name);
		start = now_us();
		fd = glfs_creat(d->fs, file_name, O_WRONLY | O_CREAT, 0644);
		if (fd == NULL) {
			fprintf(stderr, "failed to create file: %s\n", strerror(errno));
			++failure;
			continue;
		}
		now = now_us();
		creat_time = now - start;

		if (d->file_size > 0) {
			ret = glfs_write(fd, data, d->file_size, 0);
			if (ret == 0 || ret != d->file_size) {
				fprintf(stderr, "failed to write file: %s\n", strerror(errno));
				++failure;
				continue;
			}
		}
		write_time = now_us() - now;
		now = now_us();

		ret = glfs_close(fd);
		if (ret == -1) {
			fprintf(stderr, "failed to close file: %s\n", strerror(errno));
			++failure;
			continue;
		}
		close_time = now_us() - now;
		now = now_us();
		one_file_total_time = now - start;
		total_time += one_file_total_time;
		printf("%02d %s %ld %ld %ld %ld\n", 
				d->worker_no,
				file_name, 
				creat_time, write_time, 
				close_time,
				one_file_total_time);
		++success;
	}
	printf("%d %ld ms\n", success, total_time/1000l);
}

int write_all_data(const void* buf, int size, FILE* fp) {
	int remain = size;
	int ret = 0;
	while (1) {
		ret = fwrite(buf, 1, remain, fp);
		if (ret < 0)
			return ret;
		remain -= ret;
		if (remain == 0)
			return size;
	}
}

void mnt_write_files(struct api_write_data* d) 
{
	int i = 0;
	int ret = 0;
	char file_name[128];
	char* data;
	int64_t start = 0;
	int64_t now = 0;
	int64_t creat_time = 0;
	int64_t write_time = 0;
	int64_t close_time = 0;
	int64_t one_file_total_time = 0;
	int64_t total_time = 0;
	int success = 0;
	int failure = 0;
	FILE* fd;

	file_name[0] = '.';
	data = (char*)malloc(d->file_size);
	read_urandom(data, d->file_size);
	for (; i < d->file_count; ++i) {
		generate_rand_file_name(file_name+1);
		start = now_us();
		fd = fopen(file_name, "wb");
		if (fd == NULL) {
			fprintf(stderr, "failed to create file: %s\n", strerror(errno));
			++failure;
			continue;
		}
		now = now_us();
		creat_time = now - start;

		if (d->file_size > 0) {
			ret = write_all_data(data, d->file_size, fd);
			if (ret == 0 || ret != d->file_size) {
				fprintf(stderr, "failed to write file: %s\n", strerror(errno));
				++failure;
				continue;
			}
		}
		write_time = now_us() - now;
		now = now_us();

		ret = fclose(fd);
		if (ret == -1) {
			fprintf(stderr, "failed to close file: %s\n", strerror(errno));
			++failure;
			continue;
		}
		close_time = now_us() - now;
		now = now_us();
		one_file_total_time = now - start;
		total_time += one_file_total_time;
		printf("%02d %s %ld %ld %ld %ld\n", 
				d->worker_no,
				file_name+1, 
				creat_time, write_time, 
				close_time,
				one_file_total_time);
		++success;
	}
	printf("%d %ld ms\n", success, total_time/1000l);
}

void* write_thread_worker(void* param) {
	struct api_write_data* d = param;
	if (d->type == GLFS_API) {
		api_write_files(d);
	} else {
		chdir(d->path);
		mnt_write_files(d);
	}
	return 0;
}

struct api_read_data {
	int type;
	const char* path;
	glfs_t *fs;
	const name_t* files;
	int file_count;
	int file_size;
	int worker_no;
};

void api_read_files(struct api_read_data* td)
{
	int i = 0;
	int ret = 0;
	const char* file_name;
	char* data;
	int64_t start = 0;
	int64_t now = 0;
	int64_t creat_time = 0;
	int64_t read_time = 0;
	int64_t close_time = 0;
	int64_t one_file_total_time = 0;
	int64_t total_time = 0;
	int success = 0;
	int failure = 0;
	glfs_fd_t* fd = NULL;

	data = (char*)malloc(td->file_size);
	for (; i < td->file_count; ++i) {
		file_name = td->files[i].name;
		start = now_us();
		//printf("begin to read: %s\n", file_name);
		fd = glfs_open(td->fs, file_name, O_RDONLY);
		if (fd == NULL) {
			fprintf(stderr, "failed to create open: %s\n", strerror(errno));
			++failure;
			continue;
		}
		now = now_us();
		creat_time = now - start;

		ret = glfs_read(fd, data, td->file_size, 0);
		if (ret == 0 || ret != td->file_size) {
			fprintf(stderr, "failed to read file: %s\n", strerror(errno));
			++failure;
			continue;
		}
		read_time = now_us() - now;
		now = now_us();

		ret = glfs_close(fd);
		if (ret == -1) {
			fprintf(stderr, "failed to close file: %s\n", strerror(errno));
			++failure;
			continue;
		}
		close_time = now_us() - now;
		now = now_us();
		one_file_total_time = now - start;
		total_time += one_file_total_time;
		printf("%02d %s %ld %ld %ld %ld\n", 
				td->worker_no,
				file_name, 
				creat_time, read_time, 
				close_time,
				one_file_total_time);
		++success;
	}
	printf("%d %ld ms\n", success, total_time/1000l);
}

int read_all_data(void *buf, int size, FILE *fp) {
	int ret = 0;
	int remain = size;
	while (1) {
		ret = fread(buf, 1, remain, fp);
		if (ret < 0)
			return ret;
		remain -= ret;
		buf += ret;
		if (remain == 0)
			return size;
	}
}

void mnt_read_files(struct api_read_data* td)
{
	int i = 0;
	int ret = 0;
	const char* file_name;
	char* data;
	int64_t start = 0;
	int64_t now = 0;
	int64_t creat_time = 0;
	int64_t read_time = 0;
	int64_t close_time = 0;
	int64_t one_file_total_time = 0;
	int64_t total_time = 0;
	int success = 0;
	int failure = 0;
	FILE* fd = NULL;

	data = (char*)malloc(td->file_size);
	for (; i < td->file_count; ++i) {
		file_name = td->files[i].name+1; // skip first backslash
		start = now_us();
		//printf("begin to read: %s\n", file_name);
		
		fd = fopen(file_name, "rb");
		if (fd == NULL) {
			fprintf(stderr, "failed to create open: %s\n", strerror(errno));
			++failure;
			continue;
		}
		now = now_us();
		creat_time = now - start;

		ret = read_all_data(data, td->file_size, fd);
		if (ret == 0 || ret != td->file_size) {
			fprintf(stderr, "failed to read file: %s\n", strerror(errno));
			++failure;
			continue;
		}
		read_time = now_us() - now;
		now = now_us();

		ret = fclose(fd);
		if (ret == -1) {
			fprintf(stderr, "failed to close file: %s\n", strerror(errno));
			++failure;
			continue;
		}
		close_time = now_us() - now;
		now = now_us();
		one_file_total_time = now - start;
		total_time += one_file_total_time;
		printf("%02d %s %ld %ld %ld %ld\n", 
				td->worker_no,
				td->files[i].name, 
				creat_time, read_time, 
				close_time,
				one_file_total_time);
		++success;
	}
	printf("%d %ld ms\n", success, total_time/1000l);
}


void* read_thread_worker(void* param) {
	struct api_read_data* d = param;
	if (d->type == GLFS_API) {
		api_read_files(d);
	} else {
		chdir(d->path);
		mnt_read_files(d);
	}
	return 0;
}

struct test_config {
	int type;
	int is_write;
	int worker_count;
	int file_size; 
	int file_count;
	const char* file_list;
	const char* server;
	const char* volume;
	const char* path;
	int verbose;
};

int parse_config(int argc, char**argv, struct test_config* cfg) {
	int c = 0;

	while ((c = getopt(argc, argv, "rw:s:n:f:xv")) != -1) {
		//printf("opt: %d, optind: %d, optarg: %s\n", c, optind, optarg);
		switch (c) {
			case 'r':
				cfg->is_write = 0;
				//printf("-r found\n");
				break;
			case 'w':
				cfg->worker_count = atoi(optarg);
				//printf("-w found\n");
				break;
			case 's':
				cfg->file_size = atoi(optarg);
				//printf("-s found\n");
				break;
			case 'n':
				cfg->file_count = atoi(optarg);
				//printf("-n found\n");
				break;
			case 'f':
				cfg->file_list = optarg;
				//printf("-f found\n");
				break;
			case 'x':
				cfg->type = MOUNT;
				break;
			case 'v':
				cfg->verbose = 1;
				break;
			default:
				fprintf(stderr, "invalid options: %d - %s\n", c, optarg);
				return -1;
		}
	}

	if (cfg->file_size < 0 
			|| cfg->file_count <= 0 
			|| cfg->worker_count <= 0) {
		fprintf(stderr, "invalid file_size/file_count/worker_count\n");
		return -1;
	}

	if (!cfg->is_write && cfg->file_list == NULL) {
		fprintf(stderr, "which files to read?\n");
		return -1;
	}

	if (cfg->type == GLFS_API) {
		if (optind > argc-2) {
			fprintf(stderr, "you must specify server and volume\n");
			return -1;
		}
		cfg->server = argv[optind++];
		cfg->volume = argv[optind];
		if (strlen(cfg->server) == 0 || strlen(cfg->volume) == 0) {
			fprintf(stderr, "empty server or volume\n");
			return -1;
		}
	} else {
		if (optind > argc-1) {
			fprintf(stderr, "you must specify mount point\n");
			return -1;
		}
		cfg->path = argv[optind++];
		if (strlen(cfg->path) == 0) {
			fprintf(stderr, "empty mount point\n");
			return -1;
		}
	}

	return 0;
}

glfs_t *open_fs(const struct test_config* cfg) {
	glfs_t *fs = NULL;

	int ret = 0;
	// printf("==============================\n");
	fs = glfs_new(cfg->volume);
	if (!fs) {
		fprintf(stderr, "glfs_new: returned NULL\n");
		exit(-1);
	}

	ret = glfs_set_volfile_server(fs, "tcp", cfg->server, 24007);
	/* 7: debug
	 * 3: error 
	 */
	if (cfg->verbose) {
		ret = glfs_set_logging(fs, "/dev/stderr", 7); 
	} else {
		ret = glfs_set_logging(fs, "/dev/stderr", 3); 
	}
	ret = glfs_init(fs);
	if (ret != 0) {
		fprintf (stderr, "glfs_init: returned %d\n", ret);
		exit(-1);
	}

	return fs;
}

void run_write_test(const struct test_config* cfg) {
	glfs_t* fs = NULL;
	int i = 0;

	pthread_t *workers = malloc(sizeof(pthread_t)*cfg->worker_count);
	struct api_write_data *datas = malloc(sizeof(struct api_write_data)*cfg->worker_count);

	if (cfg->type == GLFS_API) 
		fs = open_fs(cfg);

	for (i = 0; i < cfg->worker_count; ++i) {
		datas[i].type = cfg->type;
		datas[i].path = cfg->path;
		datas[i].fs = fs;
		datas[i].file_size = cfg->file_size;
		datas[i].file_count = cfg->file_count/cfg->worker_count;
		datas[i].worker_no = i+1;
		pthread_create(&workers[i], NULL,  write_thread_worker, &datas[i]);
	}

	for (i = 0; i < cfg->worker_count; ++i) {
		pthread_join(workers[i], NULL);
	}

	if (cfg->type == GLFS_API) 
		glfs_fini(fs);

	free(workers);
	free(datas);
}

void run_read_test(const struct test_config* cfg) {
	const int MAXLINE = 1024;
	char buf[MAXLINE];
	glfs_t* fs = NULL;
	int i = 0;
	int total = 0;
	int each = 0;

	FILE *fp = fopen(cfg->file_list, "rb");
	name_t *files = malloc(sizeof(name_t) * cfg->file_count);
	for (total = 0; total < cfg->file_count; ) {
		if (fgets(buf, MAXLINE, fp) == NULL)
			break;

		// check thread id "00 "
		if (strlen(buf) < 42 || buf[3] != '/')
			continue;

		// skip thread id
		strncpy(files[total].name, buf+3, 39);
		files[total].name[39] = '\0';
		++total;
	}

	each = total / cfg->worker_count;

	pthread_t *workers = malloc(sizeof(pthread_t)*cfg->worker_count);
	struct api_read_data *datas = malloc(sizeof(struct api_read_data)*cfg->worker_count);

	if (cfg->type == GLFS_API)
		fs = open_fs(cfg);

	for (i = 0; i < cfg->worker_count; ++i) {
		datas[i].type = cfg->type;
		datas[i].fs = fs;
		datas[i].path = cfg->path;
		datas[i].file_size = cfg->file_size;
		datas[i].file_count = cfg->file_count/cfg->worker_count;
		datas[i].files = files + i * each;
		datas[i].worker_no = i+1;
		pthread_create(&workers[i], NULL,  read_thread_worker, &datas[i]);
	}

	for (i = 0; i < cfg->worker_count; ++i) {
		pthread_join(workers[i], NULL);
	}

	if (cfg->type == GLFS_API)
		glfs_fini(fs);

	free(files);
	free(workers);
	free(datas);
}

void print_config(struct test_config *cfg) {
	char now[12];
	now_string(now);
	if (cfg->is_write) {
		if (cfg->type == GLFS_API) {
			printf("%s API_WRITE: -w %d -s %d -n %d %s %s\n", now, cfg->worker_count, 
					cfg->file_size, cfg->file_count, cfg->server, cfg->volume);
		} else {
			printf("%s MNT_WRITE: -w %d -s %d -n %d %s\n", now, cfg->worker_count, 
					cfg->file_size, cfg->file_count, cfg->path);
		}
	} else {
		if (cfg->type == GLFS_API) {
			printf("%s API_READ: -w %d -s %d -n %d -f %s %s %s\n", now, cfg->worker_count, 
					cfg->file_size, cfg->file_count, cfg->file_list, cfg->server, cfg->volume);
		} else {
			printf("%s MNT_READ: -w %d -s %d -n %d -f %s %s\n", now, cfg->worker_count, 
					cfg->file_size, cfg->file_count, cfg->file_list, cfg->path);
		}
	}
}

int main(int argc, char**argv) 
{
	int64_t start;
	char now[128];
	struct test_config cfg;
	cfg.type = GLFS_API;
	cfg.is_write = 1;
	cfg.file_size = 0;
	cfg.file_count = 0;
	cfg.file_list = NULL;
	cfg.server = NULL;
	cfg.volume = NULL;
	cfg.path = NULL;
	cfg.verbose = 0;

	if (parse_config(argc, argv, &cfg) != 0) {
		//fprintf(stderr, "usage: test.exe -t 1 -w 5 -s 1024 -n 100000 x2 dht1vol\n");
		return -1;
	}

	print_config(&cfg);


	start = now_us();

	if (cfg.is_write) {
		run_write_test(&cfg);
	} else {
		run_read_test(&cfg);
	}

	now_string(now);
	printf("%s - %ld s \n", now, (now_us() - start)/1000/1000);

	return 0;
}

