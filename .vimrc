let g:ycm_global_ycm_extra_conf = "./ycm_extra_conf.py"

let NERDTreeIgnore=['\.o$', '\.ko$', '\.symvers$', '\.order$', '\.mod.c$', '\.swp$', '\.bak$', '\~$', '\.la', '\.am', '\.in', '\.lo', '\.o']

let NERDTreeSortOrder=['\/$', 'Makefile', 'makefile', '*', '\~$']
let NERDTreeMinimalUI=1
let NERDTreeQuitOnOpen=0
let NERDTreeWinPos = 'right'
let NERDTreeWinSize = 31
autocmd VimEnter * NERDTree
wincmd w
" move cursor from NERDTree to file
autocmd VimEnter * wincmd w 

set gdbprg=gdb\ --args\ /home/gfs-admin/glusterfs/api/examples/open_files_test\ -w\ 1\ -s\ 4096\ -n\ 10\ x2\ afr2vol
set path+=/usr/include/glusterfs,/include/glusterfs
let g:ctrlp_match_window = 'top'

set makeprg=make\ -C\ /home/gfs-admin/glusterfs/api/examples/\ open_files_test

" autocmd VimEnter * edit ./api/examples/glfsxmp.c
" autocmd VimEnter * split

" donot consider -> . as word sepatator, so we can use <cword> to select 
" a->b, or a.b
set iskeyword+=45,46,62


