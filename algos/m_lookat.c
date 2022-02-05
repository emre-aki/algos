/*
 *  m_lookat.c
 *  algos
 *
 *  Created by Emre Akı on 2022-01-30.
 *
 *  SYNOPSIS:
 *      A module for carrying out 3-D camera transformations--analogous to those
 *      in `gluLookAt`
 */

#include <stdio.h>
#include <math.h>

#include "e_malloc.h"
#include "m_lookat.h"

int SIZE_MAT_4 = sizeof(mat4_t);

static double M_Norm3 (vec3_t v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

static vec3_t M_Scale3 (vec3_t v, double k)
{
    v.x *= k; v.y *= k; v.z *= k;
    return v;
}

/* TODO: maybe use `Q_rsqrt` to calculate inverse square roots??
 * https://en.wikipedia.org/wiki/Fast_inverse_square_root
 */
static vec3_t M_Normalize3 (vec3_t v)
{
    double norm = M_Norm3(v);
    v.x /= norm; v.y /= norm; v.z /= norm;
    return v;
}

static vec3_t M_Sub3 (vec3_t u, vec3_t v)
{
    u.x -= v.x; u.y -= v.y; u.z -= v.z;
    return u;
}

static vec3_t M_Add3 (vec3_t u, vec3_t v)
{
    vec3_t vnegate = { -v.x, -v.y, -v.z };
    return M_Sub3(u, vnegate);
}

static double M_Dot3 (vec3_t u, vec3_t v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

static double M_Dot4 (vec4_t u, vec4_t v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

static vec3_t M_Cross3 (vec3_t u, vec3_t v)
{
    vec3_t w = { u.y * v.z - u.z * v.y,
                 u.z * v.x - u.x * v.z,
                 u.x * v.y - u.y * v.x };
    return w;
}

static mat4_t* M_QuickInv (mat4_t* m)
{
    mat4_t* inv = (mat4_t*) E_Malloc(SIZE_MAT_4, M_QuickInv);
    /* we can hack our way here and decompose the original transform matrix `m`
     * as such:
     *
     *   m = T * R
     *
     *       | 1 0 0 tx |   | x.x y.x z.x 0 |
     *     = | 0 1 0 ty | * | x.y y.y z.y 0 |
     *       | 0 0 1 tz |   | x.z y.z z.z 0 |
     *       | 0 0 0 1  |   | 0   0   0   1 |
     *
     * decomposing this way, we can invert `T` and `R` separately:
     *
     *   m^(-1) = R^(-1) * T^(-1)
     *
     *            | x.x x.y x.z 0 |   | 1 0 0 -t.x |
     *          = | y.x y.y y.z 0 | * | 0 1 0 -t.y |
     *            | z.x z.y z.z 0 |   | 0 0 1 -t.z |
     *            | 0   0   0   1 |   | 0 0 0  1   |
     *
     * because the basis vectors `x`, `y`, and `z` (the first 3 columns in `R`)
     * are guaranteed to be orthonormal, we were able to take a shortcut in
     * calculating the inverse by simply transposing `R`. likewise, to invert
     * the translation matrix `T`, all we need to do is scale the translation
     * vector `t` (the final column in `T`) by `-1`.
     *
     * this gives us the inverted transform:
     *
     *            | x.x x.y x.z -<t, x> |
     *   m^(-1) = | y.x y.y y.z -<t, y> |
     *            | z.x z.y z.z -<t, z> |
     *            | 0   0   0    1      |
     */
    /* invert `R` */
    inv->x.x = m->x.x; inv->x.y = m->y.x; inv->x.z = m->z.x; inv->x.w = m->w.x;
    inv->y.x = m->x.y; inv->y.y = m->y.y; inv->y.z = m->z.y; inv->y.w = m->w.y;
    inv->z.x = m->x.z; inv->z.y = m->y.z; inv->z.z = m->z.z; inv->z.w = m->w.z;
    /* invert `T` */
    inv->w.x = -M_Dot4(m->x, m->w); inv->w.y = -M_Dot4(m->y, m->w);
    inv->w.z = -M_Dot4(m->z, m->w); inv->w.w = 1;
    return inv;
}

mat4_t* M_LookAt (vec3_t* eye, vec3_t* center, vec3_t* up)
{
    mat4_t* pointat = (mat4_t*) E_Malloc(SIZE_MAT_4, M_LookAt);
    vec3_t centerv = *center, eyev = *eye, upv = *up;
    // calculate the `z` axis for the camera, normalized
    vec3_t camz = M_Normalize3(M_Sub3(centerv, eyev));
    // calculate the `y` axis for the camera, normalized:
    // because the `up` vector in the arguments does not necessarily have to be
    // perpendicular to the `z` axis, we have to adjust it to make sure the
    // orthonormality is preserved
    vec3_t camy = M_Normalize3(M_Sub3(upv, M_Scale3(camz, M_Dot3(camz, upv))));
    // calculate the `x` axis for the camera:
    // a simple cross product of `y` & `z` axes (mind the order) will give us
    // the `x` axis for the camera
    vec3_t camx = M_Cross3(camy, camz);
    /* populate the `pointat` matrix with the orthonormal base vectors for the
     * camera
     */
    pointat->x.x = camx.x; pointat->x.y = camx.y;
    pointat->x.z = camx.z; pointat->x.w = 0;
    pointat->y.x = camy.x; pointat->y.y = camy.y;
    pointat->y.z = camy.z; pointat->y.w = 0;
    pointat->z.x = camz.x; pointat->z.y = camz.y;
    pointat->z.z = camz.z; pointat->z.w = 0;
    /* the last column in the `pointat` determines the translation of the camera
     * in world space
     */
    pointat->w.x = eye->x; pointat->w.y = eye->y;
    pointat->w.z = eye->z; pointat->w.w = 1;
    // since we need to transform the world space into the camera space, i.e.,
    // move an entire world with respect to the camera, we actually need the
    // inverse of the `pointat` matrix
    mat4_t* lookat = M_QuickInv(pointat);
    E_Free(pointat); // we no longer need the `pointat`--don't forget to free it
    return lookat;
}

void M_Debug (mat4_t* m)
{
    printf("Matrix @%p:\n\n", m);

    if (!m) { printf("NULL\n"); return; }

    printf("%.3f %.3f %.3f %.3f\n", m->x.x, m->y.x, m->z.x, m->w.x);
    printf("%.3f %.3f %.3f %.3f\n", m->x.y, m->y.y, m->z.y, m->w.y);
    printf("%.3f %.3f %.3f %.3f\n", m->x.z, m->y.z, m->z.z, m->w.z);
    printf("%.3f %.3f %.3f %.3f\n", m->x.w, m->y.w, m->z.w, m->w.w);
}
