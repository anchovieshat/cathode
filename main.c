void outb(int port, char val) {
	__asm__("outb %0, %w1" : : "a"(val), "Nd"(port));
}

void print(const char *something) {
	while (*something != 0) {
		outb(0x3f8, *(something++));
	}
}

void die(void) {
	print("died");
}

typedef struct fat_bs {
	unsigned char bootjmp[3];
	unsigned char oem_name[8];
	unsigned short bytes_per_sector;
	unsigned char sectors_per_cluster;
	unsigned short reserved_sector_count;
	unsigned char table_count;
	unsigned short root_entry_count;
	unsigned short total_sectors_16;
	unsigned char media_type;
	unsigned short table_size_16;
	unsigned short sectors_per_track;
	unsigned short head_side_count;
	unsigned int hidden_sector_count;
	unsigned int total_sectors_32;

	unsigned char extended[54];
} __attribute__((packed)) fat_bs_t;

typedef struct fat32 {
	unsigned int sectors_per_fat;
	unsigned short flags;
	unsigned short version;
	unsigned int root_cluster;
	unsigned short fsinfo_sect;
	unsigned short backup_bs_sect;
	unsigned char reserved_0[12];
	unsigned char drive_number;
	unsigned char reserved_1;
	unsigned char sig;
	unsigned int volume_id;
	unsigned char volume_label[11];
	unsigned char fat_type_label[8];
} __attribute__((packed)) fat32_t;

typedef struct fat_de {
	unsigned char filename[11];
	unsigned char attr;
	unsigned char _reserved;
	unsigned char create_secs;
	unsigned short create_time;
	unsigned short create_date;
	unsigned short last_access;
	unsigned short cluster_hi;
	unsigned short last_mod_time;
	unsigned short last_mod_date;
	unsigned short cluster_lo;
	unsigned int size;
} __attribute__((packed)) fat_de_t;

typedef struct dap {
	unsigned char size;
	unsigned char _reserved;
	unsigned short blocks;
	unsigned int buf_ptr;
	unsigned long long lba;
} __attribute__((packed)) dap_t;

void do_read(struct dap *);

void read_disk(unsigned long long lba, unsigned int dest) {
	struct dap dap;
	dap.size = 0x10;
	dap._reserved = 0;
	dap.blocks = 1;
	dap.buf_ptr = dest;
	dap.lba = lba;
	do_read(&dap);
}

void main(void) {
	int root_cluster;
	fat_bs_t *fat_bs = (fat_bs_t*)0x8000;
	fat32_t *fat32 = (fat32_t*)fat_bs->extended;

	read_disk(0, 0x8000);

	root_cluster = fat32->root_cluster;

	read_disk(root_cluster, 0x9000);
}

