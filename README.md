node.c
======

a c based data tree for diverse uses in all kinds of applications (includes imports,exports)
  features a very robust and very fast single-pass json parser visiting every byte in the stream only once except numbers parsing.
   (no memory leakage or crashes as far as i can tell )


nyx
===

a node.c based scripting language with a pretty unique feature "it uses no keywords"

Example:

/*a brainfuck interpreter written in nyx*/

program = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";

while(_exp,_do)={?(_exp,_do,{},1)};

ip = 0;
program_len = program.len();
println("program length:"+program_len);
core=[];
ptr=0;

while({ip<program_len},{
	opcode = program[ip];
	(opcode==">").do({ptr++});
	(opcode=="<").do({ptr--});
	(opcode=="+").do({core[ptr]++});
	(opcode=="-").do({core[ptr]--});
	(opcode==".").do({print(core[ptr].char())});
	(opcode==",").do({core[ptr]=input().from_char();});
	(opcode=="[").do({
		(core[ptr]==0).do({
			open=0;
			?({(program[++ip]!="]")||(open)},{?({program[ip]=="["},{open++},{},0);?({program[ip]=="]"},{open--},{},0);},{},1);
			});
		});
	(opcode=="]").do({
		(core[ptr]).do({
			open=0;
			?({(program[--ip]!="[")||(open)},{?({program[ip]=="]"},{open++},{},0);?({program[ip]=="["},{open--},{},0);},{},1);
			});
		});
	ip++;
});

/*end of nyx brainfuck interpreter*/

BUILD = 6609

[![Build Status](https://buildhive.cloudbees.com/job/pez2001/job/node.c/badge/icon)](https://buildhive.cloudbees.com/job/pez2001/job/node.c/)

