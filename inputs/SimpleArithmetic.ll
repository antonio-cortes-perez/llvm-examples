; ModuleID = 'SimpleArithmetic.ll'
source_filename = "SimpleArithmetic.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local i32 @simpleArithmetic(i32 %A, i32 %B, i32 %C, i32 %D) #0 {
entry:
  %add = add nsw i32 %B, %C
  %sub = sub nsw i32 %add, %D
  %add1 = add nsw i32 %sub, %C
  %sub2 = sub nsw i32 %add, %D
  ret i32 %sub2
}

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 11.0.1-++20201103062553+ef4ffcafbb2-1~exp1~20201103053207.128"}
