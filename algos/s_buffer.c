/*
 *  s_buffer.c
 *  algos
 *
 *  Created by Emre Akı on 2024-09-17.
 *
 *  SYNOPSIS:
 *      A rather unique implementation of the ubiquitous S-Buffer, once a very
 *      popular alternative to Z-Buffering for solving the hidden surface
 *      removal problem in software rendering.
 *
 *      The implementation uses a binary tree instead of a linked list to cut
 *      down on the search time. It also supports self-balancing following each
 *      insertion to keep the depth of the tree at a minimum.
 *
 *      Original FAQ by Paul Nettle:
 *      https://www.gamedev.net/articles/programming/graphics/s-buffer-faq-r668/
 */

#include <stdio.h>

#include "e_malloc.h"
#include "s_buffer.h"
#include "t_typedef.h"

#define MAX(a, b) ((((a) > (b)) * (a)) + (((b) >= (a)) * (b)))

#define BF(n) (((n)->next ? ((n)->next->depth + 1) : 0) - \
               ((n)->prev ? ((n)->prev->depth + 1) : 0))

// FIXME: mitigate the `MAX` usage here
#define DEP(n) (MAX((n)->prev ? ((n)->prev->depth + 1) : 0, \
                    (n)->next ? ((n)->next->depth + 1) : 0))

static span_t* SB_Span (int start, int size, byte id)
{
    span_t* node = (span_t*) E_Malloc(sizeof(span_t), SB_Span);

    node->prev = 0;
    node->next = 0;
    node->start = start;
    node->size = size;
    node->depth = 0;
    node->id = id;

    return node;
}

sbuffer_t* SB_Init (int size, size_t max_depth)
{
    sbuffer_t* sbuffer = (sbuffer_t*) E_Malloc(sizeof(sbuffer_t), SB_Init);

    sbuffer->root = 0;
    sbuffer->size = size;
    sbuffer->max_depth = max_depth;

    return sbuffer;
}

/* UNUSED: Recursive version 🤕 */
static
int
_SB_Push_Rec
( span_t* node,
  int start, int size,
  int left, int right,
  byte id,
  byte* pushed )
{
    // clipping boundaries for the previous (left) subtree
    const int prevleft = left, prevright = node->start;
    // clipping boundaries for the next (right) subtree
    const int nextleft = node->start + node->size, nextright = right;
    // total size remaining after all the clipping
    int remaining = size;

    /* left-insertion: node to insert is to the left of the current node */
    if (start < node->start)
    {
        /* previous subtree is occupied, continue searching for an empty slot */
        if (node->prev)
        {
            remaining = _SB_Push_Rec(node->prev,
                                     start, size,
                                     prevleft, prevright,
                                     id,
                                     pushed);
        }
        /* found an empty slot */
        else
        {
            // clip the current segment from left
            const int clipleft = MAX(prevleft - start, 0);
            // ...and right
            const int clipright = MAX(start + size - prevright, 0);
            remaining -= clipleft + clipright;

            /* only insert if there's something left to insert */
            if (remaining > 0)
            {
                node->prev = SB_Span(start + clipleft, remaining, id);
                // update the pointer to `pushed` because we've just pushed
                // something into the buffer
                *pushed = 0xff;
            }

            // update the size remaining after the clipping
            remaining = size - clipleft - remaining;
        }

        // is there anything left from the segment after the left-insertion
        // that we should still proceed with a right-insertion?
        int stillremaining = remaining - node->size;

        if (stillremaining > 0)
        {
            /* next subtree is occupied, continue searching for an empty slot */
            if (node->next)
            {
                remaining = _SB_Push_Rec(node->next,
                                         nextleft, stillremaining,
                                         nextleft, nextright,
                                         id,
                                         pushed);
            }
            /* found an empty slot */
            else
            {
                // clipping from the right only is sufficient in this case, as
                // the start of the insertion always touches the end of parent
                const int clipright = MAX(nextleft + stillremaining - nextright,
                                          0);
                stillremaining -= clipright;

                if (stillremaining > 0)
                {
                    node->next = SB_Span(nextleft, stillremaining, id);
                    // update the pointer to `pushed` because we've just pushed
                    // something into the buffer
                    *pushed = 0xff;
                }

                // update the size remaining after the clipping
                remaining -= node->size + stillremaining;
            }
        }
    }
    /* right-insertion: node to insert is to the right of the current node */
    else
    {
        /* next subtree is occupied, continue searching for an empty slot */
        if (node->next)
        {
            remaining = _SB_Push_Rec(node->next,
                                     start, size,
                                     nextleft, nextright,
                                     id,
                                     pushed);
        }
        /* found an empty slot */
        else
        {
            // clip the current segment from left
            const int clipleft = MAX(nextleft - start, 0);
            // ...and right
            const int clipright = MAX(start + size - nextright, 0);
            remaining -= clipleft + clipright;

            if (remaining > 0)
            {
                node->next = SB_Span(start + clipleft, remaining, id);
                // update the pointer to `pushed` because we've just pushed
                // something into the buffer
                *pushed = 0xff;
            }

            // update the size remaining after the clipping
            remaining = size - clipleft - remaining;
        }
    }

    return remaining;
}

/* UNUSED: Recursive version 🤕 */
static int SB_Push_Rec (sbuffer_t* sbuffer, int start, int size, byte id)
{
    /* the buffer is empty */
    if (!sbuffer->root)
    {
        // clip the current segment from left
        const int clipleft = MAX(-start, 0);
        // ...and right
        const int clipright = MAX(start + size - sbuffer->size, 0);
        const int remaining = size - clipleft - clipright;

        /* only insert if there's something left to insert */
        if (remaining > 0)
        {
            sbuffer->root = SB_Span(start + clipleft, remaining, id);

            return 0;
        }

        return 1;
    }

    // a flag to indicate whether we've been able to insert at least a portion
    // of the segment into the buffer, or if the space the segment should be
    // inserted into has already completely occluded
    byte pushed = 0;
    _SB_Push_Rec(sbuffer->root, start, size, 0, sbuffer->size, id, &pushed);

    if (!pushed)
    {
        printf("[SB_Push_Rec] Cannot add more segments, spot fully occluded!\n");

        return 1;
    }

    return 0;
}

typedef struct {
    span_t* node;
    int     left, right;
} pscope_t;

int SB_Push (sbuffer_t* sbuffer, int start, int size, byte id)
{
    span_t* curr = sbuffer->root;

    /* the buffer is empty — initialize the root and return immediately */
    if (!curr)
    {
        // clip the segment from left
        const int clipleft = MAX(-start, 0);
        // ...and right
        const int clipright = MAX(start + size - sbuffer->size, 0);
        const int clipped_size = size - clipleft - clipright;

        /* only insert if there's something left to insert */
        if (clipped_size > 0)
        {
            sbuffer->root = SB_Span(start + clipleft, clipped_size, id);

            return 0;
        }

        return 1;
    }

    // left and right boundaries of insertion
    int left = 0, right = sbuffer->size;
    // where the insertion starts, and how wide the remaining segment is
    int offset = start, remaining = size;
    byte pushed = 0; // whether we were able push to anything
    /* initialize the push-stack to store the local scope for each "recursive"
     * stride
     */
    pscope_t stack[sbuffer->max_depth];
    int depth = 0; // stack pointer: how deep into the tree we currently are

    /* continue pushing in sub-segments unless there's nothing left to insert */
    while (remaining > 0)
    {
        span_t* parent;

        /* try to find an available spot to insert */
        while (curr)
        {
            if (depth == sbuffer->max_depth)
            {
                printf("[SB_Push] Maximum buffer depth reached!\n");

                return 1;
            }

            parent = curr;
            pscope_t scope = { parent, left, right };
            *(stack + depth++) = scope;

            if (offset < parent->start)
            {
                right = parent->start;
                curr = parent->prev;
            }
            else
            {
                left = parent->start + parent->size;
                curr = parent->next;
            }
        }
        /* we should have found an empty spot to insert by now */

        // clip the current sub-segment from left
        const int clipleft = MAX(left - offset, 0);
        // ...and right
        const int clipright = MAX(offset + remaining - right, 0);
        const int clipped_size = remaining - clipleft - clipright;

        /* only insert if there's something left to insert */
        if (clipped_size > 0)
        {
            curr = SB_Span(offset + clipleft, clipped_size, id);
            if (offset < parent->start) parent->prev = curr;
            else parent->next = curr;
            pushed = 0xff;
        }

        // where to continue inserting should there be any remaining
        // sub-segments
        int insertion_bookmark = -1;
        // where the imbalance occurred, if one did occur
        int imbalance_bookmark = -1;
        // temporary stack pointer to walk back up the stack
        int stack_depth = depth - 1;
        // temporary offset to determine whether there had been a left turn
        // while walking back up the stack
        int tmp_start = offset;

        /* trace the insertion stack back in reverse to see if we need to
         * continue inserting remaining segments, or if we need to re-balance
         * the buffer...
         */
        for (size_t i = 0; i < depth; ++i)
        {
            /* ...until we've found both a left turn and an imbalanced node */
            if (!(insertion_bookmark < 0 || (curr && imbalance_bookmark < 0)))
                break;

            pscope_t scope = *(stack + stack_depth);
            span_t* parent_node = scope.node;

            /* remember "where we left off" for the next iteration:
             * we only care about left turns, as they are the ones that can
             * potentially leave outstanding sub-segments yet to be inserted
             */
            if (insertion_bookmark < 0 && tmp_start < parent_node->start)
                insertion_bookmark = stack_depth;
            tmp_start = parent_node->start;

            if (imbalance_bookmark < 0 && curr)
            {
                const int balance_factor = BF(parent_node);
                /* remember where the imbalance occurred, if there happened to
                 * be one...
                 */
                if (balance_factor < -1 || balance_factor > 1)
                    imbalance_bookmark = stack_depth;
                /* ...otherwise, update the depth of this node */
                else
                    /* FIXME: there might not be a need to use `MAX` here due to
                     * the intrinsic nature of how AVL trees grow, i.e.,
                     * `depth - stack_depth` should always be greater than or
                     * equal to `node->depth`
                     */
                    parent_node->depth = MAX(parent_node->depth,
                                             depth - stack_depth);
            }

            --stack_depth;
        }

        /* update the scope parameters if we are to continue inserting */
        if (insertion_bookmark >= 0)
        {
            pscope_t scope = *(stack + insertion_bookmark);
            curr = scope.node;
            left = scope.left;
            right = scope.right;
            offset = curr->start + curr->size;
            // there's an outstanding sub-segment of size `clipright` waiting to
            // be inserted — try to insert it to the right of the current node
            remaining = clipright - curr->size;
            // adjust the stack pointer for the next iteration
            depth = insertion_bookmark;
        }
        /* if not, then we're free to exit */
        else
        {
            remaining = 0;
        }

        /* lo and behold: *the* balancing, at long last!
         * let's balance the crap out of this buffer, shall we?
         * here goes nothing...
         */
        if (imbalance_bookmark >= 0)
        {
            /* remember the parent of where the imbalance started, you're gonna
             * need it later
             */
            span_t* imbalance_parent = 0;
            if (imbalance_bookmark)
                imbalance_parent = (stack + imbalance_bookmark - 1)->node;

            span_t* old_parent = (stack + imbalance_bookmark)->node;
            span_t *child, *new_parent;

            /* restore balance in the `prev` sub-tree */
            if (BF(old_parent) < 0)
            {
                new_parent = old_parent->prev;
                child = new_parent->prev;

                if (BF(new_parent) > 0) // need to do a double-rotation
                {
                    child = new_parent;
                    new_parent = child->next;
                    child->next = new_parent->prev;
                    new_parent->prev = child;
                    old_parent->prev = new_parent;
                }

                old_parent->prev = new_parent->next;
                new_parent->next = old_parent;
            }
            /* restore balance in the `next` sub-tree */
            else
            {
                new_parent = old_parent->next;
                child = new_parent->next;

                if (BF(new_parent) < 0) // need to do a double-rotation
                {
                    child = new_parent;
                    new_parent = child->prev;
                    child->prev = new_parent->next;
                    new_parent->next = child;
                    old_parent->next = new_parent;
                }

                old_parent->next = new_parent->prev;
                new_parent->prev = old_parent;
            }

            /* update the depths after balancing */
            old_parent->depth = DEP(old_parent);
            child->depth = DEP(child);
            new_parent->depth = DEP(new_parent);

            /* update the parent of the newly balanced node */
            if (imbalance_parent)
            {
                if (new_parent->start < imbalance_parent->start)
                    imbalance_parent->prev = new_parent;
                else
                    imbalance_parent->next = new_parent;
            }
            /* if there is no parent, it means we just balanced the root node,
             * so update its reference
             */
            else
            {
                sbuffer->root = new_parent;
            }

            /* re-construct the stack after having balanced the buffer, only if
             * the balancing had occurred higher up the stack than where we
             * should continue inserting from
             */
            if (imbalance_bookmark <= insertion_bookmark)
            {
                int i = imbalance_bookmark;
                int new_left = 0, new_right = sbuffer->size;

                /* re-adjust the initial `left` and `right` boundaries unless
                 * the imbalance occurred at the root
                 */
                if (i)
                {
                    const pscope_t* scope = stack + i - 1;
                    const span_t* parent_node = scope->node;
                    new_left = scope->left;
                    new_right = scope->right;

                    if (new_parent->start < parent_node->start)
                        new_right = parent_node->start;
                    else
                        new_left = parent_node->start + parent_node->size;
                }

                for (span_t* stack_node = new_parent; stack_node; ++i)
                {
                    pscope_t scope = { stack_node, new_left, new_right };
                    *(stack + i) = scope;

                    // we've reached the "insertion bookmark", the
                    // re-construction of the stack is complete
                    if (stack_node->id == curr->id) break;

                    if (offset < stack_node->start)
                    {
                        new_right = stack_node->start;
                        stack_node = stack_node->prev;
                    }
                    else
                    {
                        new_left = stack_node->start + stack_node->size;
                        stack_node = stack_node->next;
                    }
                }

                left = new_left;   // update the `left`...
                right = new_right; // ...and the `right` boundaries
                depth = i; // adjust the stack pointer for the next iteration
            }
        }
    }

    if (!pushed)
    {
        printf("[SB_Push] Cannot add more segments, spot fully occluded!\n");

        return 1;
    }

    return 0;
}

static void _SB_Dump (span_t* node, size_t depth)
{
    size_t indent = depth << 2;
    for (size_t i = 0; i < indent; ++i) printf(" ");
    printf("[%c] %d + %d\n", node->id, node->start, node->size);
    if (node->prev) _SB_Dump(node->prev, depth + 1);
    if (node->next) _SB_Dump(node->next, depth + 1);
}

void SB_Dump (sbuffer_t* sbuffer)
{
    span_t* root = sbuffer->root;
    if (!root)
    {
        printf("[SB_Dump] Empty S-Buffer!\n");

        return;
    }

    _SB_Dump(root, 0);
}

static void _SB_Print (span_t* node, byte* span)
{
    int offset = node->start;

    for (int i = 0; i < node->size; ++i) *(span + offset++) = node->id;
    if (node->prev) _SB_Print(node->prev, span);
    if (node->next) _SB_Print(node->next, span);
}

void SB_Print (sbuffer_t* sbuffer)
{
    int spansize = sbuffer->size;
    byte span[spansize + 1];

    *(span + spansize) = 0;
    for (int i = 0; i < spansize; ++i) *(span + i) = '_';
    if (sbuffer->root) _SB_Print(sbuffer->root, span);
    printf("%s\n", span);
}

void SB_Destroy (sbuffer_t* sbuffer)
{
    span_t *curr = sbuffer->root, *parent;
    span_t* stack[sbuffer->max_depth];
    size_t i = 0;

    while (curr)
    {
        while (curr)
        {
            parent = curr;
            *(stack + i++) = parent;
            curr = parent->prev;
        }
        /* `prev` sub-trees have been exhausted at this point */

        curr = parent->next; // try the `next` sub-tree

        /* no `prev` or `next` sub-trees mean we're on a leaf node, go ahead and
         * free it
         */
        if (!curr)
        {
            span_t* grandparent = 0;

            if (--i > 0)
            {
                grandparent = *(stack + --i);
                /* remove the link from the grandparent to the parent, so we
                 * won't end up back here going back up the stack
                 */
                if (parent->start < grandparent->start) grandparent->prev = 0;
                else grandparent->next = 0;
            }

            E_Free(parent);
            curr = grandparent; // continue freeing from the grandparent
        }
    }

    E_Free(sbuffer);
}
