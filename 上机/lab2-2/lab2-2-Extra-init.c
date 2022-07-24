static void page_migrate_test(){
	struct Page *pp;
	page_alloc(&pp);
	Pde *pgdir = (Pde*)page2kva(pp);
	page_alloc(&pp);
	page_insert(pgdir, pp, 0x23300000, 0);
	page_insert(pgdir, pp, 0x23400000, 0);
	page_insert(pgdir, pp, 0x23500000, 0);
	pp = page_migrate(pgdir, pp);
	printf("%d\n", page2ppn(pp));
	pp = page_migrate(pgdir, pp);
	printf("%d\n", page2ppn(pp));
}

void mips_init(){
	mips_detect_memory();
	mips_vm_init();
	page_init();

	page_migrate_test();

	*((volatile char*)(0xB0000010)) = 0;
}
