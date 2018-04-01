#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef unsigned char       kl_u8_t;
typedef signed char         kl_s8_t;
typedef unsigned short      kl_u16_t;
typedef signed short        kl_s16_t;
typedef unsigned long       kl_u32_t;
typedef signed long         kl_s32_t;
typedef char                kl_char_t;
typedef float               kl_float_t;
typedef double              kl_double_t;
typedef kl_s16_t            kl_ssize_t;
typedef kl_u16_t            kl_size_t;

typedef kl_u32_t            kernel_time_t;
typedef kl_u32_t            kernel_msec_t;
typedef kl_u32_t            kernel_usec_t;
typedef double              kernel_sec_t;

typedef enum{
    False=0,
    True=!False,
}kl_bool_t;

typedef struct kernel_timeval{
    kernel_time_t tv_sec;
    kernel_usec_t tv_usec;
}kernel_timeval_t;


#define KL_NULL    0

    
    
#if defined(__ICCARM__)
#define __kl_volatile  volatile
#define __kl_inline    static inline
#define __kl_weak      __weak
#elif defined(__GNUC__)
#define __kl_volatile  volatile
#define __kl_inline    static __inline
#define __kl_weak      __attribute__((weak))
#elif defined(__CC_ARM)
#define __kl_volatile  __volatile
#define __kl_inline    static __inline
#define __kl_weak      __weak
#endif

#define ENO_OK                          0               /**< There is no error */
#define ENO_ERROR                       1               /**< A generic error happens */
#define ENO_TIMEOUT                     2               /**< Timed out */
#define ENO_FULL                        3               /**< The resource is full */
#define ENO_EMPTY                       4               /**< The resource is empty */
#define ENO_NOMEM                       5               /**< No memory */
#define ENO_NOSYS                       6               /**< No system */
#define ENO_BUSY                        7               /**< Busy */
#define ENO_IO                          8               /**< IO error */
#define ENO_PARAM                       9


#define container_of(node, type, member)\
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))

#define kernel_bit(b)    (1<<(b))

#define kernel_bit_set(x,b)            ((x)|=kernel_bit(b))
#define kernel_bit_reset(x,b)          ((x)&=~(kernel_bit(b)))
#define kernel_bits_set(x,bits)        ((x)|=bits)
#define kernel_bits_reset(x,bits)      ((x)&=~(bi/bits))
#define kernel_test_bit(x,b)           (((x)&(kernel_bit(b)))?1:0)
#define kernel_test_bits(x,bits)       (((x)&(bits))?1:0)
    
struct klist_head{
    struct klist_head *next;
    struct klist_head *prev;
};

typedef struct klist_head klist_t;

struct kernel_device{
    const char *name;
    int flags;
    const struct device_operations *fops;
    void *private_data;
};
typedef struct kernel_device kernel_device_t;

struct device_operations{
    int (*init)(struct kernel_device *dev);
    int (*open)(struct kernel_device *dev, int flags);
    int (*close)(struct kernel_device *dev);
    int (*read)(struct kernel_device *dev, kl_u8_t *buf, int len);
    int (*write)(struct kernel_device *dev, const kl_u8_t *buf, int len);
    int (*cntl)(struct kernel_device *dev, int cmd, void *arg);
};



#ifdef __cplusplus
}
#endif
#endif