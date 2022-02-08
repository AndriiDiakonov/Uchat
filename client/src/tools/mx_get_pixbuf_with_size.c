#include "../../inc/uchat_client.h"

GdkPixbuf *mx_get_pixbuf_with_size(char *path, int w, int h) {
    GdkPixbuf *pixbuf = mx_create_pixbuf(path);
    GdkPixbuf *result = gdk_pixbuf_scale_simple(GDK_PIXBUF(pixbuf), w, h, GDK_INTERP_BILINEAR);
    g_object_unref(G_OBJECT(pixbuf));
    return result;
}
