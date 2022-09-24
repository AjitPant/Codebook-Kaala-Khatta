  {
 "shell_cmd": "",
     "file_regex": "^(..[^:]*):([0-9]+):?([0-9]+)?:? (.*)$",
     "working_dir": "${file_path}",
     "selector": "source.c, source.c++",
     "variants":
     [
      
       {
         "name": "Run_debug_main",
         "shell_cmd": "g++ -std=c++17 -Wshadow -Wall  -fsanitize=address,undefined -static-libasan  -g3 -fno-omit-frame-pointer   -fmax-errors=2 \"${file}\" -o \"${file_path}/${file_base_name}\" &&    \"${file_path}/${file_base_name}\" < \"${file_path}/input.txt\" > \"${file_path}/output.txt\" ",
       },
       {
         "name": "Run_fast",
         "shell_cmd": "g++ -std=c++17 -Ofast -Wl,-z,stack-size=412943040  -fmax-errors=2 \"${file}\" -o \"${file_path}/${file_base_name}\" &&    \"${file_path}/${file_base_name}\" < input.txt > output.txt",
       },
     ]

    }
