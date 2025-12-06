#ifndef __KCEJ_SD_EE_H__
#define __KCEJ_SD_EE_H__

#ifdef __cplusplus
extern "C" {
#endif

extern int sd_init(void);
extern void sd_set_cli(int sound_code);
extern void *sd_status(void);

#ifdef __cplusplus
}
#endif
#endif // {{{ END OF FILE }}}
