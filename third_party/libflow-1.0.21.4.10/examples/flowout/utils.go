package main

import (
	"strings"
	"time"
)

//////////////////////////////////////////////////////////////////////
// Data types
//////////////////////////////////////////////////////////////////////

func Min(a, b int) int {
	if a < b {
		return a
	} else {
		return b
	}
}

func IsStringInArray(str string, array []string) bool {
	for _, s := range array {
		if s == str {
			return true
		}
	}

	return false
}

func IsStringWildcardMatchArray(str string, array []string) bool {
	for _, s := range array {
		if WildcardMatch(s, str) {
			return true
		}
	}

	return false
}

//////////////////////////////////////////////////////////////////////
// Wildcard
//////////////////////////////////////////////////////////////////////

// MatchSimple - finds whether the text matches/satisfies the pattern string.
// supports only '*' wildcard in the pattern.
// considers a file system path as a flat name space.
func WildcardMatchSimple(pattern, name string) bool {
	if pattern == "" {
		return name == pattern
	}
	if pattern == "*" {
		return true
	}
	rname := make([]rune, 0, len(name))
	rpattern := make([]rune, 0, len(pattern))
	for _, r := range name {
		rname = append(rname, r)
	}
	for _, r := range pattern {
		rpattern = append(rpattern, r)
	}
	simple := true // Does only wildcard '*' match.
	return WildcardDeepMatchRune(rname, rpattern, simple)
}

// Match -  finds whether the text matches/satisfies the pattern string.
// supports  '*' and '?' wildcards in the pattern string.
// unlike path.Match(), considers a path as a flat name space while matching the pattern.
// The difference is illustrated in the example here https://play.golang.org/p/Ega9qgD4Qz .
func WildcardMatch(pattern, name string) (matched bool) {
	if pattern == "" {
		return name == pattern
	}
	if pattern == "*" {
		return true
	}
	rname := make([]rune, 0, len(name))
	rpattern := make([]rune, 0, len(pattern))
	for _, r := range name {
		rname = append(rname, r)
	}
	for _, r := range pattern {
		rpattern = append(rpattern, r)
	}
	simple := false // Does extended wildcard '*' and '?' match.
	return WildcardDeepMatchRune(rname, rpattern, simple)
}

func WildcardDeepMatchRune(str, pattern []rune, simple bool) bool {
	for len(pattern) > 0 {
		switch pattern[0] {
		default:
			if len(str) == 0 || str[0] != pattern[0] {
				return false
			}
		case '?':
			if len(str) == 0 && !simple {
				return false
			}
		case '*':
			return WildcardDeepMatchRune(str, pattern[1:], simple) ||
				(len(str) > 0 && WildcardDeepMatchRune(str[1:], pattern, simple))
		}
		str = str[1:]
		pattern = pattern[1:]
	}
	return len(str) == 0 && len(pattern) == 0
}

//////////////////////////////////////////////////////////////////////
// Parsers
//////////////////////////////////////////////////////////////////////

func ParseCommaSeparatedStrings(str string) (parts []string) {
	unique_parts := make(map[string]bool)
	for _, part := range strings.Split(str, ",") {
		if part != "" {
			_, exist := unique_parts[part]
			if !exist {
				parts = append(parts, part)
				unique_parts[part] = true
			}
		}
	}

	return
}

//////////////////////////////////////////////////////////////////////
// Time
//////////////////////////////////////////////////////////////////////

func AfterSeconds(nsecs int) time.Time {
	return time.Now().Add(time.Second * time.Duration(nsecs))
}

func MakeTimeFromMicroSeconds(usec int64) time.Time {
	sec := usec / 1000000
	nsec := usec % 1000000 * 1000
	return time.Unix(sec, nsec)
}
