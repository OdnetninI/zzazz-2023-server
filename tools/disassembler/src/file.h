#ifndef __FILE_H__
#define __FILE_H__

#pragma once

#ifdef FILE_IMP
#define extern_ 
#else
#define extern_ extern
#endif

extern_ FILE* input;
extern_ FILE* output;

#endif /* __FILE_H__ */