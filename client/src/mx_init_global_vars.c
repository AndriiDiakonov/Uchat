#include "../inc/uchat_client.h"

void mx_init_global_vars(void) {
    THEME = 0;
    argv_ptr = NULL;
    sockfd = 0;
    sock_for_rooms = 0;
    sock_for_send = 0;
    counter = 0;
    // Containers
    window = NULL;
    main_area = NULL;
    authorization_area = NULL;
    chat_area = NULL;
    chats_list = NULL;
    settings_menu = NULL;
    blackout = NULL;
    error_revealer = NULL;
    tools_menu = NULL;
    search_menu = NULL;
    edit_prev = NULL;

    // Chat variables
    curr_room_msg_head = NULL;
    chats_list_head = NULL;
    search_list_head = NULL;
    t_chat_room_vars.message_enter_area = NULL;
    t_chat_room_vars.right_container = NULL;
    t_chat_room_vars.more_box = NULL;
    selected_msg_widget = NULL;
    selected_msg_struct = NULL;
    check_messages_id = 0;
    max_msg_id = 0;

    // Temporary variables for edit user form
    t_edit_user.NewFirstName = NULL;
    t_edit_user.NewSecondName = NULL;
    t_edit_user.NewPseudonim = NULL;
    t_edit_user.NewDescription = NULL;
    t_edit_user.NewAvatar = NULL;

    language = 0;
    text_for_labels = NULL;
}
