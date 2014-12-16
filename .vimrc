
let NERDTreeIgnore=['\.o$', '\.ko$', '\.symvers$', '\.order$', '\.mod.c$', '\.swp$', '\.bak$', '\~$', '\.la', '\.in', '\.lo', '\.o']

let NERDTreeSortOrder=['\/$', 'Makefile', 'makefile', '*', '\~$']
let NERDTreeMinimalUI=1
let NERDTreeQuitOnOpen=0
let NERDTreeWinPos = 'right'
let NERDTreeWinSize = 31
autocmd VimEnter * NERDTree
wincmd w
" move cursor from NERDTree to file
autocmd VimEnter * wincmd w 

" set gdbprg=gdb\ --args\ /root/work/glusterfs/glusterfsd/src/glusterfs\ -s\ x2\ --volfile-id\ gluster/nfs
set gdbprg=gdb\ --args\ /usr/local/sbin/glusterd
set gdbprg=gdb\ --args\ /usr/local/sbin/gluster\ volume\ info
set gdbprg=gdb\ --args\ /root/work/glusterfs/api/examples/open_files_test\ -w\ 1\ -s\ 4096\ -n\ 1\ \-v\ x2\ afrvol
set gdbprg=gdb\ --args\ /usr/local/sbin/glusterd\ -N
set gdbprg=gdb\ --args\ /root/work/glusterfs/api/examples/open_files_test\ -w\ 1\ -s\ 4096\ -n\ 2\ \-v\ 192.168.80.101\ afr1vol
set gdbprg=gdb\ --args\ /root/work/glusterfs/api/examples/open_files_test\ -w\ 1\ -s\ 4096\ -n\ 2\ \-v\ -r\ -f\ api/examples/test.log\ 192.168.80.101\ afr1vol
let g:ctrlp_match_window = 'top'

set makeprg=make\ -C\ /root/work/glusterfs/api/examples/\ open_files_test

" autocmd VimEnter * edit ./api/examples/glfsxmp.c
" autocmd VimEnter * split

" donot consider -> . as word sepatator, so we can use <cword> to select 
" a->b, or a.b
set iskeyword+=45,46,62
set tabstop=8

function! NERDTreeCustomIgnoreFilter(path)
    return a:path.isExecutable
endfunction

set path+=libglusterfs/src
set path+=api/src
set path+=heal/src
set path+=geo-replication/src
set path+=xlators/nfs/server/src
set path+=xlators/performance/readdir-ahead/src
set path+=xlators/performance/io-threads/src
set path+=xlators/performance/io-cache/src
set path+=xlators/performance/read-ahead/src
set path+=xlators/performance/quick-read/src
set path+=xlators/performance/md-cache/src
set path+=xlators/performance/write-behind/src
set path+=xlators/performance/open-behind/src
set path+=xlators/performance/symlink-cache/src
set path+=xlators/storage/posix/src
set path+=xlators/storage/bd/src
set path+=xlators/mgmt/glusterd/src
set path+=xlators/system/posix-acl/src
set path+=xlators/mount/fuse/src
set path+=xlators/encryption/crypt/src
set path+=xlators/encryption/rot-13/src
set path+=xlators/cluster/dht/src
set path+=xlators/cluster/map/src
set path+=xlators/cluster/stripe/src
set path+=xlators/cluster/ha/src
set path+=xlators/cluster/afr/src
set path+=xlators/features/path-convertor/src
set path+=xlators/features/protect/src
set path+=xlators/features/quota/src
set path+=xlators/features/mac-compat/src
set path+=xlators/features/gfid-access/src
set path+=xlators/features/marker/src
set path+=xlators/features/filter/src
set path+=xlators/features/compress/src
set path+=xlators/features/changelog/lib/src
set path+=xlators/features/changelog/src
set path+=xlators/features/trash/src
set path+=xlators/features/qemu-block/src
set path+=xlators/features/index/src
set path+=xlators/features/locks/src
set path+=xlators/features/glupy/src
set path+=xlators/features/read-only/src
set path+=xlators/features/quiesce/src
set path+=xlators/protocol/auth/login/src
set path+=xlators/protocol/auth/addr/src
set path+=xlators/protocol/server/src
set path+=xlators/protocol/client/src
set path+=xlators/lib/src
set path+=xlators/playground/template/src
set path+=xlators/debug/error-gen/src
set path+=xlators/debug/io-stats/src
set path+=xlators/debug/trace/src
set path+=xlators/meta/src
set path+=rpc/xdr/src
set path+=rpc/rpc-transport/socket/src
set path+=rpc/rpc-transport/rdma/src
set path+=rpc/rpc-lib/src
set path+=glusterfs-hadoop/0.20.2/src
set path+=glusterfsd/src
set path+=cli/src

if filereadable(".NERDTreeBookmarks")
    let g:NERDTreeBookmarksFile = ".NERDTreeBookmarks"
endif
