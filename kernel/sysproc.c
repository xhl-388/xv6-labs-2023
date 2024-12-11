#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  uint64 va;
  int npages;
  uint64 out;
  unsigned int bitmask = 0;
  struct proc *cproc = myproc();
  pte_t *tmpPte = 0;

  argaddr(0, &va);
  argint(1, &npages);
  argaddr(2, &out);

  vmprint(cproc->pagetable);
  pte_t* pte = walk(cproc->pagetable, va, 0);
  if (0 == pte){
    printf("try to get access bit from a invalid virtual address");
    goto bad;
  }
  for(int i =0; i < npages && i < 32; i++){
    tmpPte = &(pte[i]);
    if((*tmpPte) & PTE_A){
      bitmask |= (1 << i);
      (*tmpPte) &= (~PTE_A);
    }
  }

  copyout(cproc->pagetable, out, (char*)&bitmask, sizeof(bitmask));
  return 0;

bad:
  bitmask = 0;
  copyout(cproc->pagetable, out, (char*)&bitmask, sizeof(bitmask));
  return -1;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
