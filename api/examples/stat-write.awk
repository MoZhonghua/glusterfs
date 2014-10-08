#! awk -f

BEGIN {
	count = 0
	total_open = 0
	total_write = 0
	total_close = 0
	total_total = 0

	max_open = 0
	max_write = 0
	max_close = 0

	interval = 500*1000
	limit = 100000000
};

/^[0-9][0-9] \// {
	if ($3 < limit) {
		count += 1
		total_open += $3
		total_write += $4
		total_close += $5
		total_total += $6
	}

	open_group[int($3/interval)] += 1
	write_group[int($4/interval)] += 1
	close_gropu[int($5/interval)] += 1

	if ($3 > max_open)
		max_open = $3
	if ($4 > max_write)
		max_write = $4
	if ($5 > max_close)
		max_close = $5
};

END {
	printf("Files count: %d\n", count)
	printf("total: %.1f(%.2f) %.1f(%.2f) %.1f(%.2f) %.1f\n", 
		   total_open/1000, total_open/total_total*100,
		   total_write/1000, total_write/total_total*100,
		   total_close/1000, total_close/total_total*100,
		   total_total/1000)
	printf("ave  : %.1f %.1f %.1f %.1f\n", total_open/count/1000, total_write/count/1000, 
		   total_close/count/1000, total_total/count/1000)

	printf("max  : %d %d %d\n", max_open, max_write, max_close)
	printf("files per second per thread: %.1f\n", 1000*1000/(total_total/count))
	#for (i in open_group) {
	#	printf("[%05d, %05d] - %d\n", i*interval/1000, (i+1)*interval/1000, open_group[i])
	#}
};

