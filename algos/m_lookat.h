/*
 *  m_lookat.h
 *  algos
 *
 *  Created by Emre Akı on 2022-01-30.
 *
 *  SYNOPSIS:
 *      A module for carrying out 3-D camera transformations--analogous to those
 *      in `gluLookAt`
 */

#ifndef m_lookat_h

#define m_lookat_h
#define m_lookat_h_vec3_t vec3_t
#define m_lookat_h_vec4_t vec4_t
#define m_lookat_h_mat4_t mat4_t
#define m_lookat_h_M_LookAt M_LookAt
#define m_lookat_h_M_Debug M_Debug

/* TODO: maybe move these to `t_typedef.h`?? */
typedef struct {
    double x;
    double y;
    double z;
} vec3_t;

typedef struct {
    double x;
    double y;
    double z;
    double w;
} vec4_t;

typedef struct {
    vec4_t x;
    vec4_t y;
    vec4_t z;
    vec4_t w;
} mat4_t;

mat4_t* M_LookAt (vec3_t* eye, vec3_t* center, vec3_t* up);
void M_Debug (mat4_t* m);

#endif
