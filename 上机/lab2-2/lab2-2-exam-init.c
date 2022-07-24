static void inverted_page_lookup_test(){
	struct Page *pp;
	page_alloc(&pp);
	Pde *pgdir = (Pde*)page2kva(pp);
	extern struct Page *pages;
	int a[10], len, i;
	page_insert(pgdir, pages + 2333, 0x23500000, 0);
	page_insert(pgdir, pages + 2333, 0x23400000, 0);
	page_insert(pgdir, pages + 2333, 0x23300000, 0);
	printf("%d\n", len = inverted_page_lookup(pgdir, pages + 2333, a));
	for(i = 0;i < len;i++) printf("%x\n", a[i]);
	page_remove(pgdir, 0x23400000);
	printf("%d\n", len = inverted_page_lookup(pgdir, pages + 2333, a));
	for(i = 0;i < len;i++) printf("%x\n", a[i]);
	page_insert(pgdir, pages + 2334, 0x23300000, 0);
	printf("%d\n", len = inverted_page_lookup(pgdir, pages + 2333, a));
	for(i = 0;i < len;i++) printf("%x\n", a[i]);
	printf("%d\n", len = inverted_page_lookup(pgdir, pages + 2334, a));
	for(i = 0;i < len;i++) printf("%x\n", a[i]);
}

void mips_init(){
	mips_detect_memory();
	mips_vm_init();
	page_init();

	inverted_page_lookup_test();

	*((volatile char*)(0xB0000010)) = 0;
}

