#ifndef __DLIST_H
#define __DLIST_H


/*  一、 核心底层宏：地址偏移量计算 */

/**
 * offsetof - 计算结构体成员在结构体中的字节偏移量
 * @TYPE:   结构体的类型
 * @MEMBER: 结构体中的成员名
 */
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

/**
 * container_of - 通过结构体内部成员的指针，获取整个大结构体的首地址
 * @ptr:    指向结构体内部成员的指针
 * @type:   大结构体的类型
 * @member: 该成员在大结构体里的名字
 */
#define container_of(ptr, type, member) ({                      \
        const __typeof__( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

/*  核心解析    0：
 它代表内存地址 0x00000000（也就是空指针的地址）。
 (TYPE *)0：
 欺骗编译器：“请你假设在内存地址 0 这个位置上，存放着一个类型为 TYPE 的结构体。”
 ((TYPE *)0)->MEMBER：
 指示编译器：“去访问这个位于地址 0 的虚拟结构体里面的 MEMBER 成员。”
 &(((TYPE *)0)->MEMBER)：
 这是最巧妙的一步。 加上 & 取地址符，表示：“我不是要读取这个成员里的数据，我只要你告诉我这个成员当前的内存地址是多少。”
 数学逻辑：成员的实际地址 = 结构体的首地址 + 成员的内部偏移量。
 因为我们强行把结构体的首地址设定成了 0，所以：成员的实际地址 = 0 + 偏移量 = 偏移量。
 (size_t)：
 最后，把计算出来的这个地址（本质上就是一个指针）强制转换为一个标准的无符号整数类型 size_t，方便我们后续做加减运算。*/

/* 二、 基础数据结构与初始化 */

/*
 * 毒指针：当节点被从链表中删除后，将其指针指向这些非法的内存地址。
 * 用于验证没有人使用未初始化的链表条目。
 */
#define LIST_POISON1  ((void *) 0x00100100)
#define LIST_POISON2  ((void *) 0x00200)

// 内核链表的指针域
struct list_head {
    struct list_head *next, *prev;
};

// 静态初始化宏：用于在声明时直接初始化一个链表头
#define LIST_HEAD_INIT(name) { &(name), &(name) }

// 声明并静态初始化一个链表头节点
#define LIST_HEAD(name) \
struct list_head name = LIST_HEAD_INIT(name)

// 动态初始化宏：在运行时将一个已存在的 list_head 节点的指针指向自己
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0) 


/*  三、 节点的添加操作 */

/*
 * __list_add - 内部函数：在两个已知连续节点之间插入一个新节点
 */
static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/**
 * list_add – 头插法
 * @new:  要添加的新条目
 * @head: 要在其后添加的链表头
 * 说明：常用于实现栈。
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

/**
 * list_add_tail – 尾插法
 * @new:  要添加的新条目
 * @head: 要在其前添加的链表头
 * 说明：常用于实现队列。
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}


/*  四、 节点的删除与移动操作 */

/*
 * __list_del - 内部函数：通过让前后节点互相指向对方来删除一个节点
 */
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

/**
 * list_del – 删除节点
 * @entry: 要从链表中删除的元素
 * 注意：删除后该节点处于未定义状态。
 */
static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    entry->next = (void *) 0;
    entry->prev = (void *) 0;
}

/**
 * list_del_init – 删除节点并重新初始化它
 * @entry: 要从链表中删除的元素
 */
static inline void list_del_init(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry);
}

/**
 * list_move – 从一个链表中删除并作为另一个链表的头部添加
 * @list: 要移动的条目
 * @head: 将在我们的条目之前的头部
 */
static inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add(list, head);
}

/**
 * list_move_tail – 从一个链表中删除并作为另一个链表的尾部添加
 * @list: 要移动的条目
 * @head: 将在我们的条目之后的头部
 */
static inline void list_move_tail(struct list_head *list, struct list_head *head)
{
    __list_del(list->prev, list->next);
    list_add_tail(list, head);
}


/*  五、 链表状态与拼接操作*/

/**
 * list_empty – 测试链表是否为空
 * @head: 要测试的链表
 */
static inline int list_empty(struct list_head *head)
{
    return head->next == head;
}

/*
 * __list_splice - 内部函数：合并两个链表
 */
static inline void __list_splice(struct list_head *list, struct list_head *head)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;
    struct list_head *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

/**
 * list_splice – 连接两个链表
 * @list: 要添加的新链表
 * @head: 在第一个链表中添加它的位置
 */
static inline void list_splice(struct list_head *list, struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head);
}

/**
 * list_splice_init – 连接两个链表并重新初始化被清空的链表
 * @list: 要添加的新链表
 * @head: 在第一个链表中添加它的位置
 */
static inline void list_splice_init(struct list_head *list, struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head);
        INIT_LIST_HEAD(list);
    }
}


/*  六、 提取数据与遍历宏 */

/**
 * list_entry – 获取此条目的结构体
 * 通过游标指针ptr减去大结构体type中的成员计算出大结构体首地址
 * @ptr:    &struct list_head 指针
 * @type:   嵌入此内容的结构体类型
 * @member: 结构体中 list_struct 的名称
 */
#define list_entry(ptr, type, member) \
((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

/**
 * list_for_each - 遍历链表
 * @pos:  用作循环计数器的 &struct list_head
 * @head: 链表的头部
 */
#define list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_prev - 向后遍历链表
 * @pos:  用作循环计数器的 &struct list_head
 * @head: 链表的头部
 */
#define list_for_each_prev(pos, head) \
for (pos = (head)->prev; pos != (head); pos = pos->prev)

/**
 * list_for_each_safe - 遍历链表，防止链表条目被删除
 * @pos:  用作循环计数器的 &struct list_head
 * @n:    另一个用作临时存储的 &struct list_head
 * @head: 链表的头部
 */
#define list_for_each_safe(pos, n, head) \
for (pos = (head)->next, n = pos->next; pos != (head); \
pos = n, n = pos->next)

/**
 * list_for_each_prev_safe - 向后遍历（安全模式）
 */
#define list_for_each_prev_safe(pos, n, head) \
for (pos = (head)->prev, n = pos->prev; pos != (head); \
pos = n, n = pos->prev)

/**
 * list_for_each_entry - 遍历给定类型的链表
 * @pos:    用作循环计数器的 type *
 * @head:   链表的头部
 * @member: 结构体中 list_struct 的名称
 */
#define list_for_each_entry(pos, head, member)                \
for (pos = list_entry((head)->next, __typeof__(*pos), member);    \
     &pos->member != (head);                                  \
     pos = list_entry(pos->member.next, __typeof__(*pos), member))

/**
 * list_for_each_entry_safe - 遍历给定类型的链表，防止链表条目被删除
 * @pos:    用作循环计数器的 type *
 * @n:      另一个用作临时存储的 type *
 * @head:   链表的头部
 * @member: 结构体中 小结构体list_struct 的名称
 */
#define list_for_each_entry_safe(pos, n, head, member)            \
for (pos = list_entry((head)->next, __typeof__(*pos), member),        \
     n = list_entry(pos->member.next, __typeof__(*pos), member);      \
     &pos->member != (head);                                      \
     pos = n, n = list_entry(n->member.next, __typeof__(*n), member))

#endif /* __DLIST_H */