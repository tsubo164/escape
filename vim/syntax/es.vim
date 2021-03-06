" Quit when a syntax file was already loaded
if exists("b:current_syntax")
  finish
endif

" Support cpoptions
let s:cpo_save = &cpo
set cpo&vim

" Set the current syntax
let b:current_syntax = "es"

" Comment
syn region esBlockComment start=+/\*+ end=+\*/+
syn region esLineComment start=+//+ end=+$+

syn region esString start=+"+ end=+"+

syn match esNumber display "\<\d[0-9_]*\(u\=l\=\|l\=u\=\)\>"

" Keywords
syn keyword esBuiltinType          bool char short int long float double string
syn keyword esStatement            break case continue default do else for goto if label return switch vardump while new delete
syn keyword esStorageClass         var const
syn keyword esStructure            fn struct

hi def link esBlockComment         Comment
hi def link esLineComment          Comment

hi def link esString               String

hi def link esNumber               Number

hi def link esBuiltinType          Type
hi def link esStatement            Statement
hi def link esStorageClass         StorageClass
hi def link esStructure            Structure

