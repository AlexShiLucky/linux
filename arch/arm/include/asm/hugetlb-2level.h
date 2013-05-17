/*
 * arch/arm/include/asm/hugetlb-2level.h
 *
 * Copyright (C) 2014 Linaro Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _ASM_ARM_HUGETLB_2LEVEL_H
#define _ASM_ARM_HUGETLB_2LEVEL_H


static inline pte_t huge_ptep_get(pte_t *ptep)
{
	return *ptep;
}

static inline void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
				   pte_t *ptep, pte_t pte)
{
	set_pmd_at(mm, addr, (pmd_t *) ptep, __pmd(pte_val(pte)));
}

static inline pte_t pte_mkhuge(pte_t pte) { return pte; }

static inline void huge_ptep_clear_flush(struct vm_area_struct *vma,
					 unsigned long addr, pte_t *ptep)
{
	pmd_t *pmdp = (pmd_t *)ptep;
	pmd_clear(pmdp);
	flush_tlb_range(vma, addr, addr + HPAGE_SIZE);
}

static inline void huge_ptep_set_wrprotect(struct mm_struct *mm,
					   unsigned long addr, pte_t *ptep)
{
	pmd_t *pmdp = (pmd_t *) ptep;
	set_pmd_at(mm, addr, pmdp, pmd_wrprotect(*pmdp));
}


static inline pte_t huge_ptep_get_and_clear(struct mm_struct *mm,
					    unsigned long addr, pte_t *ptep)
{
	pmd_t *pmdp = (pmd_t *)ptep;
	pte_t pte = huge_ptep_get(ptep);
	pmd_clear(pmdp);

	return pte;
}

static inline int huge_ptep_set_access_flags(struct vm_area_struct *vma,
					     unsigned long addr, pte_t *ptep,
					     pte_t pte, int dirty)
{
	int changed = !pte_same(huge_ptep_get(ptep), pte);
	if (changed) {
		set_huge_pte_at(vma->vm_mm, addr, ptep, pte);
		flush_tlb_range(vma, addr, addr + HPAGE_SIZE);
	}

	return changed;
}

static inline pte_t huge_pte_mkwrite(pte_t pte)
{
	pmd_t pmd = __pmd(pte_val(pte));
	pmd = pmd_mkwrite(pmd);
	return __pte(pmd_val(pmd));
}

static inline pte_t huge_pte_mkdirty(pte_t pte)
{
	pmd_t pmd = __pmd(pte_val(pte));
	pmd = pmd_mkdirty(pmd);
	return __pte(pmd_val(pmd));
}

static inline unsigned long huge_pte_dirty(pte_t pte)
{
	return pmd_dirty(__pmd(pte_val(pte)));
}

static inline unsigned long huge_pte_write(pte_t pte)
{
	return pmd_write(__pmd(pte_val(pte)));
}

static inline void huge_pte_clear(struct mm_struct *mm, unsigned long addr,
				  pte_t *ptep)
{
	pmd_t *pmdp = (pmd_t *)ptep;
	pmd_clear(pmdp);
}

static inline pte_t mk_huge_pte(struct page *page, pgprot_t pgprot)
{
	pmd_t pmd = mk_pmd(page,pgprot);
	return __pte(pmd_val(pmd));
}

static inline pte_t huge_pte_modify(pte_t pte, pgprot_t newprot)
{
	pmd_t pmd = pmd_modify(__pmd(pte_val(pte)), newprot);
	return __pte(pmd_val(pmd));
}

static inline pte_t huge_pte_wrprotect(pte_t pte)
{
	pmd_t pmd = pmd_wrprotect(__pmd(pte_val(pte)));
	return __pte(pmd_val(pmd));
}

static inline struct page *huge_pte_page(pte_t pte)
{
	return pfn_to_page((pte_val(pte) & HPAGE_MASK) >> PAGE_SHIFT);
}

static inline unsigned long huge_pte_present(pte_t pte)
{
	return 1;
}

static inline pte_t huge_pte_mkyoung(pte_t pte)
{
	return __pte(pmd_val(pmd_mkyoung(__pmd(pte_val(pte)))));
}

#endif /* _ASM_ARM_HUGETLB_2LEVEL_H */
