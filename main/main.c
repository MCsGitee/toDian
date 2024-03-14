#include <stdio.h>
#include <uart_types.h>
#include <uart.h>

typedef struct
{
    /* data */
    int baud_rate;
    uart_word_length_t data_bits;
    uart_parity_t parity;
    uart_stop_bits_t stop_bits;
    uart_hw_flowcontrol_t flow_ctrl;
    uint8_t rx_flow_ctrl_thresh;
    union
    {
        /* data */
        uart_sclk_t source_clk;
        bool use_ref_tick __attribute__((deprecated));
    };
    
} uart_config_t;

void app_main(void)
{
    const uart_port_t uart_num= UART_NUM_2;
    uart_config_t uart_config={
        .baud_rate=115200,
        .data_bits=UART_DATA_8_BITS,
        .parity=UART_PARITY_DISABLE,
        .stop_bits=UART_STOP_BITS_1,
        .flow_ctrl=UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh=122,
    };
    ESP_ERROR_CHECK(uart_param_config(uart_num,&uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2,4,5,18,19));
    const int uart_buffer_size=(1024*2);
    QueueHandle_t uart_queue;
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2,uart_buffer_size,uart_buffer_size,10,&uart_queue,0));
    char* test_str="hello world\\n";
    uart_write_bytes(uart_num,(const char*)test_str,strlen(test_str));
    const uart_port_t uart_num=UART_NUM_2;
    uint8_t data[128];
    int length=0;
    ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num,(size_t*)&length));
    length=uart_read_bytes(uart_num,data,length,100);
}
