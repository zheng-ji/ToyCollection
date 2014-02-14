" My hi profile to change the default color.
" Date : 2008-09-24

if &background == "dark"
  hi Comment	term=bold cterm=NONE ctermfg=Cyan ctermbg=NONE gui=NONE guifg=#80a0ff guibg=NONE
  hi Constant	term=underline cterm=NONE ctermfg=Magenta ctermbg=NONE gui=NONE guifg=#ffa0a0 guibg=NONE
  hi Special	term=bold cterm=NONE ctermfg=LightRed ctermbg=NONE gui=NONE guifg=Orange guibg=NONE
  hi Identifier	term=underline cterm=bold ctermfg=Cyan ctermbg=NONE gui=NONE guifg=#40ffff guibg=NONE
  hi Statement	term=bold cterm=NONE ctermfg=Yellow ctermbg=NONE gui=bold guifg=#ffff60 guibg=NONE
  hi PreProc	term=underline cterm=NONE ctermfg=LightBlue ctermbg=NONE gui=NONE guifg=#ff80ff guibg=NONE
  hi Type		term=underline cterm=NONE ctermfg=LightGreen ctermbg=NONE gui=bold guifg=#60ff60 guibg=NONE
  hi Underlined	term=underline cterm=underline ctermfg=LightBlue gui=underline guifg=#80a0ff
  hi Ignore	term=NONE cterm=NONE ctermfg=black ctermbg=NONE gui=NONE guifg=bg guibg=NONE
else
  hi Comment	term=bold cterm=NONE ctermfg=DarkGreen ctermbg=NONE gui=NONE guifg=Blue guibg=NONE
  hi Constant	term=underline cterm=NONE ctermfg=DarkRed ctermbg=NONE gui=NONE guifg=Magenta guibg=NONE
  hi Special	term=bold cterm=NONE ctermfg=DarkMagenta ctermbg=NONE gui=NONE guifg=SlateBlue guibg=NONE
  hi Identifier	term=underline cterm=NONE ctermfg=DarkCyan ctermbg=NONE gui=NONE guifg=DarkCyan guibg=NONE
  hi Statement	term=bold cterm=NONE ctermfg=DarkBlue ctermbg=NONE gui=bold guifg=Brown guibg=NONE
  hi PreProc	term=underline cterm=NONE ctermfg=DarkMagenta ctermbg=NONE gui=NONE guifg=Purple guibg=NONE
  hi Type		term=underline cterm=NONE ctermfg=DarkBlue ctermbg=NONE gui=bold guifg=SeaGreen guibg=NONE
  hi Underlined	term=underline cterm=underline ctermfg=DarkMagenta gui=underline guifg=SlateBlue
  hi Conditional		term=underline cterm=NONE ctermfg=DarkBlue ctermbg=NONE gui=bold guifg=SeaGreen guibg=NONE
  hi Repeat		term=underline cterm=NONE ctermfg=DarkBlue ctermbg=NONE gui=bold guifg=SeaGreen guibg=NONE
  hi Ignore	term=NONE cterm=NONE ctermfg=white ctermbg=NONE gui=NONE guifg=bg guibg=NONE
endif
