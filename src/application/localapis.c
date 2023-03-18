#include <application/localapis.h>
#include <wangyonglin/object.h>
#include <wangyonglin/buffer.h>
#include <wangyonglin/string.h>
void localapis_init(localapis_t **aliapis, datasheet cfg, const char *section)
{
    if (object_create((void **)aliapis, sizeof(localapis_t)))
    {
        object_command_t objects[] = {
            object_string_command("AccessKeyId", offsetof(localapis_t, AccessKeyId)),
            object_string_command("AccessKeySecret", offsetof(localapis_t, AccessKeySecret)),
            object_string_command("ProductKey", offsetof(localapis_t, ProductKey)),
            object_string_command("DeviceName", offsetof(localapis_t, DeviceName)),
            object_string_command("Format", offsetof(localapis_t, Format)),
            object_string_command("Version", offsetof(localapis_t, Version)),
            object_string_command("SignatureMethod", offsetof(localapis_t, SignatureMethod)),
            object_string_command("SignatureVersion", offsetof(localapis_t, SignatureVersion)),
            object_string_command("RegionId", offsetof(localapis_t, RegionId)),
            object_string_command("TopicFullName", offsetof(localapis_t, TopicFullName)),
            object_null_command};
        object_mirror(objects, (*aliapis), cfg, section);
    }
}
void localapis_free(localapis_t *aliapis)
{
    buffer_delete(aliapis->AccessKeyId);
    buffer_delete(aliapis->AccessKeySecret);
    buffer_delete(aliapis->DeviceName);
    buffer_delete(aliapis->Format);
    buffer_delete(aliapis->Version);
    buffer_delete(aliapis->SignatureMethod);
    buffer_delete(aliapis->SignatureVersion);
    buffer_delete(aliapis->RegionId);
    buffer_delete(aliapis->TopicFullName);
}