
let g:ycm_global_ycm_extra_conf = "./ycm_extra_conf.py"

let NERDTreeIgnore=['\.o$', '\.ko$', '\.symvers$', '\.order$', '\.mod.c$', '\.swp$', '\.bak$', '\~$', '\.la', '\.am', '\.in', '\.lo', '\.o']

let NERDTreeSortOrder=['\/$', 'Makefile', 'makefile', '\.c$', '\.cc$', '\.cpp$', '\.h$', '*', '\~$']
let NERDTreeMinimalUI=1
let NERDTreeQuitOnOpen=0
let NERDTreeWinPos = 'right'
let NERDTreeWinSize = 31
let g:miniBufExplMinSize=1
" let g:miniBufExplSplitBelow = 1
autocmd VimEnter * NERDTree
wincmd w
" move cursor from NERDTree to file
autocmd VimEnter * wincmd w 

set gdbprg=gdb\ --args\ ./api/examples/glfsxmp\ testvol\ brick-s0
set path+=/usr/include/glusterfs,/include/glusterfs
let g:ctrlp_match_window = 'top'

