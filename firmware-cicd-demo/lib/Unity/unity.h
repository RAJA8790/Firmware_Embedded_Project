/**
 * unity.h — Minimal Unity Test Framework (self-contained for CI demo)
 * Based on Unity by ThrowTheSwitch (MIT License)
 */
#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

static int  _unity_failures = 0;
static int  _unity_tests    = 0;
static int  _unity_ignores  = 0;
static const char *_unity_test_name = "";

#define UNITY_BEGIN() \
    do { _unity_failures=0; _unity_tests=0; _unity_ignores=0; printf("\n"); } while(0)

#define UNITY_END() unity_print_summary()

#define RUN_TEST(func) \
    do { _unity_test_name=#func; _unity_tests++; \
         printf("%s:%d:%s:RUNNING\n",__FILE__,__LINE__,#func); func(); } while(0)

#define TEST_PASS() \
    printf("%s:%d:%s:PASS\n",__FILE__,__LINE__,_unity_test_name)

#define TEST_IGNORE() \
    do { _unity_ignores++; \
         printf("%s:%d:%s:IGNORE\n",__FILE__,__LINE__,_unity_test_name); return; } while(0)

#define _UNITY_FAIL(msg) \
    do { _unity_failures++; \
         printf("%s:%d:%s:FAIL: %s\n",__FILE__,__LINE__,_unity_test_name,msg); return; } while(0)

#define TEST_ASSERT(c) \
    do { if(!(c)){_unity_failures++;printf("%s:%d:%s:FAIL: assert(%s)\n",__FILE__,__LINE__,_unity_test_name,#c);return;} \
         else printf("%s:%d:%s:PASS\n",__FILE__,__LINE__,_unity_test_name); } while(0)

#define TEST_ASSERT_TRUE(c)  TEST_ASSERT((c)!=0)
#define TEST_ASSERT_FALSE(c) TEST_ASSERT((c)==0)
#define TEST_ASSERT_NULL(p)  TEST_ASSERT((p)==NULL)
#define TEST_ASSERT_NOT_NULL(p) TEST_ASSERT((p)!=NULL)

#define TEST_ASSERT_EQUAL(e,a) \
    do { if((long)(e)!=(long)(a)){_unity_failures++; \
         printf("%s:%d:%s:FAIL: Expected %ld got %ld\n",__FILE__,__LINE__,_unity_test_name,(long)(e),(long)(a));return;} \
         else printf("%s:%d:%s:PASS\n",__FILE__,__LINE__,_unity_test_name); } while(0)

#define TEST_ASSERT_EQUAL_INT(e,a)    TEST_ASSERT_EQUAL(e,a)
#define TEST_ASSERT_EQUAL_UINT8(e,a)  TEST_ASSERT_EQUAL((uint8_t)(e),(uint8_t)(a))
#define TEST_ASSERT_EQUAL_UINT16(e,a) TEST_ASSERT_EQUAL((uint16_t)(e),(uint16_t)(a))
#define TEST_ASSERT_EQUAL_UINT32(e,a) TEST_ASSERT_EQUAL((uint32_t)(e),(uint32_t)(a))

#define TEST_ASSERT_NOT_EQUAL(e,a) \
    do { if((long)(e)==(long)(a)){_unity_failures++; \
         printf("%s:%d:%s:FAIL: Should not equal %ld\n",__FILE__,__LINE__,_unity_test_name,(long)(e));return;} \
         else printf("%s:%d:%s:PASS\n",__FILE__,__LINE__,_unity_test_name); } while(0)

#define TEST_ASSERT_EQUAL_STRING(e,a) \
    do { if(strcmp((e),(a))!=0){_unity_failures++; \
         printf("%s:%d:%s:FAIL: Expected '%s' got '%s'\n",__FILE__,__LINE__,_unity_test_name,e,a);return;} \
         else printf("%s:%d:%s:PASS\n",__FILE__,__LINE__,_unity_test_name); } while(0)

static inline int unity_print_summary(void){
    printf("\n-----------------------\n");
    printf("%d Tests  %d Failures  %d Ignored\n",_unity_tests,_unity_failures,_unity_ignores);
    printf("%s\n", _unity_failures==0 ? "OK" : "FAIL");
    printf("-----------------------\n");
    return _unity_failures;
}

#endif /* UNITY_H */
