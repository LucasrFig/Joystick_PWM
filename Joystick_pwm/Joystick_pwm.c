#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/bootrom.h"

//Bibliotecas Criadas
#include "lib/Joystick_pwm.h"
#include "display/ssd1306.h"

//Definir macros
#define joyX 27
#define joyY 26
#define sensibilidade 50
#define I2C_LINK 0x3C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define BUT_A 5
#define BUT_B 6
#define BUT_J 22
#define LED_G 11
#define LED_B 12
#define LED_R 13
#define DIV 25.0
#define WRAP 1800
uint DUTY_C = 0;//Duty Cycle Inicial
uint sliceB;//Slice PWM azul
uint sliceR;//Slice PWM vermelho

//Variáveis para temporização:
uint32_t last_print_time = 0;
uint32_t last_timeA = 0;//Guarda a última vez que o botão A foi pressionado
uint32_t last_timeJ = 0;//Guarda a última vez que o botão Joystick foi pressionado
ssd1306_t ssd;
bool cor = true;
bool pwm_function = true;//Variável que permite o joystick controlar intensidade dos LEDs
bool ledg_state = false;//Indica estado atual do led verde
//bool borda = true;

static void gpio_irq_handler(uint gpio,uint32_t events){
    //Se for o botão B põe a placa no modo bootsel
    if(gpio==BUT_B){
        printf("Reiniciando a placa em modo de gravação...\n");
        reset_usb_boot(0,0);
    }

    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_timeA > 300000){ //debouncing

        last_timeA = current_time;

        if(gpio==BUT_A){
            pwm_function = !pwm_function;//inverte a variável
            pwm_set_enabled(sliceB, pwm_function); //habilita/desabilita o pwm no slice do led azul
            pwm_set_enabled(sliceR, pwm_function); //habilita/desabilita o pwm no slice do led vermelho
            printf("%s\n",pwm_function?"PWM ATIVADO":"PWM DESATIVADO");//Para debug
        }
    }
    
    if (current_time - last_timeJ > 300000){ //debouncing
        
        last_timeJ = current_time;
        
        if(gpio==BUT_J){
            ledg_state = !ledg_state;
            gpio_put(LED_G,ledg_state);
            printf("%s\n",ledg_state?"LED VERDE ATIVADO":"LED VERDE DESATIVADO");//Para debug
        }
    }

}


int main()
{
    //Inicializar sistema de entrada e saída padrão
    stdio_init_all();

    //Inicializar ADC no joystick
    joystick_adc_init(joyX,joyY);//Biblioteca joystick_pwm.h

    //Inicializa o i2c, configura a estrutura do display, limpa o display
    ssd1306_init_config_clean(&ssd,I2C_SCL,I2C_SDA,I2C_PORT,I2C_LINK);//(Função adicionada | "display/ssd136.h")


    //configura pwm dos LEDs vermelho e azul
    sliceB = gpio_pwm_config(LED_B,DIV,WRAP,DUTY_C);//Biblioteca joystick_pwm.h
    sliceR = gpio_pwm_config(LED_R,DIV,WRAP,DUTY_C);//Biblioteca joystick_pwm.h
    pwm_set_enabled(sliceB, pwm_function); //habilita o pwm no slice do led azul
    pwm_set_enabled(sliceR, pwm_function); //habilita o pwm no slice do led vermelho

    //inicializar led verde
    gpio_init(LED_G);
    gpio_set_dir(LED_G,GPIO_OUT);

    //inicializar botões e interrupções
    gpio_init(BUT_B);
    gpio_set_dir(BUT_B,GPIO_IN); gpio_pull_up(BUT_B);
    gpio_set_irq_enabled_with_callback(BUT_B, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);
    gpio_init(BUT_A);
    gpio_set_dir(BUT_A,GPIO_IN); gpio_pull_up(BUT_A);
    gpio_set_irq_enabled_with_callback(BUT_A, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);
    gpio_init(BUT_J);
    gpio_set_dir(BUT_J,GPIO_IN); gpio_pull_up(BUT_J);
    gpio_set_irq_enabled_with_callback(BUT_J, GPIO_IRQ_EDGE_FALL, 1, &gpio_irq_handler);

    
    
    while (true) {
        
        //Ler adc do joystick:
        adc_select_input(0); //Ler eixo Y
        uint16_t joyY_value = adc_read(); 
        adc_select_input(1); //Ler eixo X
        uint16_t joyX_value = adc_read(); 
        
        //Atualizar posição do quadrado no display:
        ssd1306_fill(&ssd, !cor); // Limpa o display
        ssd1306_rect(&ssd, 3,3, 124, 60, cor,!cor);//Desenha a borda
        
        uint posX = (joyX_value*115/4095)+4;//guarda o valor da pos x
        uint posY = 52-(joyY_value*48/4095);//guarda o valor da pos y
        ssd1306_rect(&ssd,posY,posX, 8, 8, cor,cor);//Atualizo com a nova posição
        ssd1306_send_data(&ssd);// Atualiza o conteúdo do display

        //Atualizar PWM dos LEDs azul e vermelho
        if(pwm_function){
            if(joyX_value>=1800 && joyX_value<=2300){
                pwm_set_gpio_level(LED_R,0);
            }else if(joyX_value > 2300){
                pwm_set_gpio_level(LED_R,joyX_value-2300);
            }else if(joyX_value <1800){
                pwm_set_gpio_level(LED_R,1800-joyX_value);
            }

            if(joyY_value>=1800 && joyY_value<=2300){
                pwm_set_gpio_level(LED_B,0);
            }else if(joyY_value > 2300){
                pwm_set_gpio_level(LED_B,joyY_value-2300);
            }else if(joyY_value <1800){
                pwm_set_gpio_level(LED_B,1800-joyY_value);
            }

        }

        //Escrever info do ADC:
        uint32_t current_time = to_ms_since_boot(get_absolute_time()); 
        if((current_time - last_print_time)>=1000){
            printf("posicao X: %d |posição Y: %d\n",posX,posY);
            printf("VRX: %u | VRY: %u\n",joyX_value,joyY_value);
            last_print_time = current_time;
        }
    }
}
