
#include "guix_round_thermostat_thread.h"
#include "lcd.h"
#include "gx_api.h"
#include "guix_round_thermostat_resources.h"
#include "guix_round_thermostat_specifications.h"

//#define            SCRATCHPAD_PIXELS (MAIN_DISPLAY_X_RESOLUTION * MAIN_DISPLAY_Y_RESOLUTION)
#define            SCRATCHPAD_PIXELS (220 * 220)
GX_COLOR           scratchpad[SCRATCHPAD_PIXELS];
TX_BYTE_POOL       memory_pool;

GX_WINDOW_ROOT * p_window_root;
GX_WINDOW p_splash_screen;

GX_CHAR           plus_text[] = "+";
GX_CHAR           minus_text[] = "-";
INT               i_temperature = 79;

void set_temperature_value(void);
void *memory_allocate(ULONG size);
void memory_free(VOID *mem);

void g_lcd_spi_callback(spi_callback_args_t * p_args);
static void guix_test_send_touch_message(sf_touch_panel_payload_t * p_payload);

void g_lcd_spi_callback(spi_callback_args_t * p_args)
{
    (void)p_args;
    tx_semaphore_ceiling_put(&g_gui_semaphore0, 1);
}

void guix_round_thermostat_thread_entry(void);

/******************************************************************************************/
void *memory_allocate(ULONG size)
{
    VOID *memptr;

    if (tx_byte_allocate(&memory_pool, &memptr, size, TX_NO_WAIT) == TX_SUCCESS)
    {
        return memptr;
    }
    return NULL;
}

/******************************************************************************************/
void memory_free(VOID *mem)
{
    tx_byte_release(mem);
}



/* GUIX Base Thread entry function */
void guix_round_thermostat_thread_entry(void)
{
ssp_err_t err;
    sf_message_header_t * p_message = NULL;
    UINT      status = TX_SUCCESS;
    /* Initializes GUIX. */
    status = gx_system_initialize();
    if(TX_SUCCESS != status)
    {
        while(1); //Debug trap
    }

    /* Initializes GUIX drivers. */
    err = g_sf_el_gx0.p_api->open (g_sf_el_gx0.p_ctrl, g_sf_el_gx0.p_cfg);
    if(SSP_SUCCESS != err)
    {
        while(1); //Debug trap
    }

    gx_studio_display_configure ( MAIN_DISPLAY,
                                  g_sf_el_gx0.p_api->setup,
                                  LANGUAGE_ENGLISH,
                                  MAIN_DISPLAY_THEME_1,
                                  &p_window_root );

    err = g_sf_el_gx0.p_api->canvasInit(g_sf_el_gx0.p_ctrl, p_window_root);
    if(SSP_SUCCESS != err)
    {
        while(1); //Debug trap
    }

    /* create byte pool for needle rotation to use */
    status = tx_byte_pool_create(&memory_pool, "scratchpad", scratchpad, SCRATCHPAD_PIXELS * sizeof(GX_COLOR));
    if(TX_SUCCESS != status)
    {
        while(1); //Debug trap
    }

    /* Creates the primary screen. */
    status = gx_studio_named_widget_create("splash_screen", (GX_WIDGET *)p_window_root, (GX_WIDGET **) &p_splash_screen);
    if(TX_SUCCESS != status)
    {
        while(1); //Debug trap
    }

    /* Shows the root window to make it visible. */
    status = gx_widget_show(p_window_root);
    if(TX_SUCCESS != status)
    {
        while(1); //Debug trap
    }

    /* Lets GUIX run. */
    status = gx_system_start();
    if(TX_SUCCESS != status)
    {
        while(1); //Debug trap
    }

    /* install our memory allocator and de-allocator */
    gx_system_memory_allocator_set(memory_allocate, memory_free);

    /** Open the SPI driver to initialize the LCD **/
    err = g_rspi_lcdc.p_api->open(g_rspi_lcdc.p_ctrl, g_rspi_lcdc.p_cfg);
    if(SSP_SUCCESS != err)
    {
        while(1); //Debug trap
    }

    /** Setup the ILI9341V **/
    ILI9341V_Init();


    while (1)
    {
        g_sf_message0.p_api->pend(g_sf_message0.p_ctrl, &guix_round_thermostat_thread_message_queue, (sf_message_header_t **) &p_message, TX_WAIT_FOREVER);
        switch (p_message->event_b.class_code)
        {
        case SF_MESSAGE_EVENT_CLASS_TOUCH:
            if (SF_MESSAGE_EVENT_NEW_DATA == p_message->event_b.code)
            {
                //sf_touch_panel_payload_t * p_touch_message = (sf_touch_panel_payload_t *) p_message;

                /** Translate a touch event into a GUIX event */
                guix_test_send_touch_message((sf_touch_panel_payload_t *) p_message);
            }
            break;
        default:
            break;
        }

        /** Message is processed, so release buffer. */
        err = g_sf_message0.p_api->bufferRelease(g_sf_message0.p_ctrl, (sf_message_header_t *) p_message, SF_MESSAGE_RELEASE_OPTION_NONE);
        if (err)
        {
            while(1); //Debug trap
        }
        tx_thread_sleep(1);
    }
}


/*******************************************************************************************************************//**
 * @brief  Sends a touch event to GUIX internal thread to call the GUIX event handler function
 *
 * @param[in] p_payload Touch panel message payload
***********************************************************************************************************************/
static void guix_test_send_touch_message(sf_touch_panel_payload_t * p_payload)
{
    bool send_event = true;
    GX_EVENT gxe;

    switch (p_payload->event_type)
    {
    case SF_TOUCH_PANEL_EVENT_DOWN:
        gxe.gx_event_type = GX_EVENT_PEN_DOWN;
        break;
    case SF_TOUCH_PANEL_EVENT_UP:
        gxe.gx_event_type = GX_EVENT_PEN_UP;
        break;
    case SF_TOUCH_PANEL_EVENT_HOLD:
    case SF_TOUCH_PANEL_EVENT_MOVE:
        gxe.gx_event_type = GX_EVENT_PEN_DRAG;
        break;
    case SF_TOUCH_PANEL_EVENT_INVALID:
        send_event = false;
        break;
    default:
        break;
    }

    if (send_event)
    {
        /** Send event to GUI */
        gxe.gx_event_sender         = GX_ID_NONE;
        gxe.gx_event_target         = 0;  /* the event to be routed to the widget that has input focus */
        gxe.gx_event_display_handle = 0;

        gxe.gx_event_payload.gx_event_pointdata.gx_point_x = (GX_VALUE)(p_payload->x);
        gxe.gx_event_payload.gx_event_pointdata.gx_point_y = (GX_VALUE)(320 - p_payload->y);

        gx_system_event_send(&gxe);
    }
}


/******************************************************************************************/
VOID custom_pixelmap_button_draw(GX_PIXELMAP_BUTTON *button)
{
INT      x_offset = 0;
INT      y_offset = 0;
GX_CHAR *text;

    gx_pixelmap_button_draw(button);


    switch (button->gx_widget_id)
    {
    case ID_PLUS_BTN:
        text = plus_text;
        break;

    case ID_MINUS_BTN:
        text = minus_text;
        break;

    }

    if (button->gx_widget_style & GX_STYLE_BUTTON_PUSHED)
    {
        x_offset++;
        y_offset++;
    }

    gx_widget_text_draw((GX_WIDGET *)button, GX_COLOR_ID_TEXT_INPUT_FILL,
                        GX_FONT_ID_BOLD, text,
                        x_offset, y_offset);
}

/******************************************************************************************/
void set_temperature_value()
{
GX_PROMPT *prompt;
static GX_CHAR str_value[10];

    prompt = &splash_screen.splash_screen_value_prompt;

    gx_utility_ltoa(i_temperature, str_value, 10);
    gx_prompt_text_set(prompt, str_value);
}

/* Splash Screen Event Handler */
/******************************************************************************************/
UINT SplashScreenEventHandler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{
    GX_CIRCULAR_GAUGE *gauge;
    INT                angle;
    UINT      status;

    gauge = &splash_screen.splash_screen_gauge;

    switch(event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_PLUS_BTN, GX_EVENT_CLICKED):
        gx_circular_gauge_angle_get(gauge, &angle);
        if (angle < 90)
        {
            status = gx_circular_gauge_angle_set(gauge, angle + 5);
            i_temperature++;
            set_temperature_value();
        }
        break;

    case GX_SIGNAL(ID_MINUS_BTN, GX_EVENT_CLICKED):
        gx_circular_gauge_angle_get(gauge, &angle);

        if (angle > -185)
        {
            status = gx_circular_gauge_angle_set(gauge, angle - 5);
            i_temperature--;
            set_temperature_value();
        }
        break;

    default:
        return gx_window_event_process(widget, event_ptr);
    }
    return 0;
}
