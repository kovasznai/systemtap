#ifndef UTT_H
#define UTT_H

enum {
	Utt_trace_setup = 1,
	Utt_trace_running,
	Utt_trace_stopped,
};

struct utt_trace {
	int trace_state;
//	struct rchan *rchan;
	struct dentry *dir;		/* systemtap/module_name */
	struct dentry *dropped_file;
	atomic_t dropped;
	struct dentry *utt_tree_root;	/* systemtap */
	void *private_data;
	atomic_t wakeup;
	struct timer_list timer;
	int is_global;
};

#define UTT_TRACE_ROOT_NAME_SIZE	64	/* Largest string for a root dir identifier */
#define UTT_TRACE_NAME_SIZE		64	/* Largest string for a trace identifier */

#ifndef UTT_TIMER_INTERVAL
#define UTT_TIMER_INTERVAL		((HZ + 99) / 100)	/* Wakeup timer interval in jiffies (default 10 ms)*/
#endif

/*
 * User setup structure
 */
struct utt_trace_setup {
	char root[UTT_TRACE_ROOT_NAME_SIZE];	/* input */
	char name[UTT_TRACE_NAME_SIZE];		/* input */
	u32 buf_size;				/* input */
	u32 buf_nr;				/* input */
	int is_global;				/* input */
	int err;				/* output */
};


static struct utt_trace *utt_trace_setup(struct utt_trace_setup *utts);

static int utt_trace_startstop(struct utt_trace *utt, int start,
			       unsigned int *trace_seq);
static void utt_trace_cleanup(struct utt_trace *utt);
static int utt_trace_remove(struct utt_trace *utt);

#ifndef STP_OLD_TRANSPORT
static size_t utt_switch_subbuf(struct utt_trace *utt, struct rchan_buf *buf,
				size_t length);
/**
 *      utt_reserve - reserve slot in channel buffer
 *      @utt: utt channel
 *      @length: number of bytes to reserve
 *
 *      Returns pointer to reserved slot, NULL if full.
 *
 *      This function is utt_switch_subbuf version of relay_reserve.
 */
static inline void *utt_reserve(struct utt_trace *utt, size_t length)
{
#if 0
	void *reserved;
	struct rchan_buf *buf = utt->rchan->buf[smp_processor_id()];

	if (unlikely(buf->offset + length > buf->chan->subbuf_size)) {
		length = utt_switch_subbuf(utt, buf, length);
		if (!length)
			return NULL;
	}
	reserved = (char*)buf->data + buf->offset;
	buf->offset += length;

	return reserved;
#else
	return NULL;
#endif
}
#endif

#endif
