#include "command.h"
#include "console.h"
#include "led.h"
#include "gpio.h"

struct cmd led_cmd;
struct cmd gpio_cmd;

int atoi(const char * str)
{
   int i;
   i=0;
   while(*str)
   {
      i = (i<<3) + (i<<1) + (*str - '0');
      str++;
   }
   return i;
}

void cmd_init(void)
{
    led_cmd.name = "led";
    led_cmd.desc = "led [options]\r\n-- Turn on/off the LED1.\r\n-- Options:\r\n---- 1: turn on the LED1\r\n---- 0: turn off the LED1";
    led_cmd.nb_arg = 0;

    gpio_cmd.name = "gpio";
    gpio_cmd.desc = "Set the pin n on the GPIO-2.";
    gpio_cmd.nb_arg = 1;
    gpio_cmd.args[0].name = "Pin number";
}

void cmd_print_usage(struct cmd cmd_struct)
{
    console_print("Usage :\r\n");
    console_print("-- ");
    console_print(cmd_struct.name);
    console_print("\r\n");
    console_print("-- ");
    console_print(cmd_struct.desc);
    console_print("\r\n");
}

void cmd_parse(const char * str)
{
   int name_len;
   name_len=0;
   char cmd_name[MAX_LEN_NAME];
   int arg_len;
   arg_len=0;
   char cmd_arg[MAX_LEN_ARG];
   while(*str && *str != ' ')
   {
	cmd_name[name_len] = *str;
	name_len++;
        str++;
   }
   cmd_name[name_len] = '\0';
   while(*str && *str == ' ')
        str++;
   while(*str && *str != ' ')
   {
        cmd_arg[arg_len] = *str;
	arg_len++;
	str++;
   }
   cmd_arg[arg_len] = '\0';
   if (console_streq(cmd_name, led_cmd.name))
   {
	int arg_value;
        led_cfg_set(1);
	if(arg_len == 0)
	    led_dat_set(!led_dat_get_state());
	else if (arg_len == 1)
	{
	    arg_value = atoi(cmd_arg);
	    if(arg_value == 1 || arg_value == 0)
		led_dat_set(arg_value);
	    else
	    {
		console_print("Bad argument : Not 0 or 1.\r\n");
		cmd_print_usage(led_cmd);
	    }
	}
	else
	{
	    console_print("Bad number of arguments\r\n");
	    cmd_print_usage(led_cmd);
	}
        console_prompt();
   }
   else if (console_streq(cmd_name, gpio_cmd.name))
   {
        gpio_output_set(9);
        gpio_activate(9);
        console_prompt();
   }
   else
   {
        console_print("Unknown command : ");
        console_print(cmd_name);
        console_print("\r\n");
        console_prompt();
   }

}
