; ModuleID = 'MatMul4x4.ll'
source_filename = "MatMul4x4.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local void @matMul4x4(i32* %A, i32* %B, i32* %C) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc17, %entry
  %IdxA.0 = phi i32 [ 0, %entry ], [ %inc18, %for.inc17 ]
  %C.addr.0 = phi i32* [ %C, %entry ], [ %C.addr.1, %for.inc17 ]
  %cmp = icmp slt i32 %IdxA.0, 4
  br i1 %cmp, label %for.body, label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.cond
  br label %for.end19

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc14, %for.body
  %IdxB.0 = phi i32 [ 0, %for.body ], [ %inc15, %for.inc14 ]
  %C.addr.1 = phi i32* [ %C.addr.0, %for.body ], [ %incdec.ptr13, %for.inc14 ]
  %cmp2 = icmp slt i32 %IdxB.0, 4
  br i1 %cmp2, label %for.body4, label %for.cond.cleanup3

for.cond.cleanup3:                                ; preds = %for.cond1
  br label %for.end16

for.body4:                                        ; preds = %for.cond1
  %mul = mul nsw i32 %IdxA.0, 4
  %idx.ext = sext i32 %mul to i64
  %add.ptr = getelementptr inbounds i32, i32* %A, i64 %idx.ext
  %idx.ext5 = sext i32 %IdxB.0 to i64
  %add.ptr6 = getelementptr inbounds i32, i32* %B, i64 %idx.ext5
  store i32 0, i32* %C.addr.1, align 4, !tbaa !2
  br label %for.cond7

for.cond7:                                        ; preds = %for.inc, %for.body4
  %RowA.0 = phi i32* [ %add.ptr, %for.body4 ], [ %incdec.ptr, %for.inc ]
  %ColB.0 = phi i32* [ %add.ptr6, %for.body4 ], [ %add.ptr12, %for.inc ]
  %Idx.0 = phi i32 [ 0, %for.body4 ], [ %inc, %for.inc ]
  %cmp8 = icmp slt i32 %Idx.0, 4
  br i1 %cmp8, label %for.body10, label %for.cond.cleanup9

for.cond.cleanup9:                                ; preds = %for.cond7
  br label %for.end

for.body10:                                       ; preds = %for.cond7
  %0 = load i32, i32* %RowA.0, align 4, !tbaa !2
  %1 = load i32, i32* %ColB.0, align 4, !tbaa !2
  %mul11 = mul nsw i32 %0, %1
  %2 = load i32, i32* %C.addr.1, align 4, !tbaa !2
  %add = add nsw i32 %2, %mul11
  store i32 %add, i32* %C.addr.1, align 4, !tbaa !2
  br label %for.inc

for.inc:                                          ; preds = %for.body10
  %inc = add nsw i32 %Idx.0, 1
  %incdec.ptr = getelementptr inbounds i32, i32* %RowA.0, i32 1
  %add.ptr12 = getelementptr inbounds i32, i32* %ColB.0, i64 4
  br label %for.cond7

for.end:                                          ; preds = %for.cond.cleanup9
  %incdec.ptr13 = getelementptr inbounds i32, i32* %C.addr.1, i32 1
  br label %for.inc14

for.inc14:                                        ; preds = %for.end
  %inc15 = add nsw i32 %IdxB.0, 1
  br label %for.cond1

for.end16:                                        ; preds = %for.cond.cleanup3
  br label %for.inc17

for.inc17:                                        ; preds = %for.end16
  %inc18 = add nsw i32 %IdxA.0, 1
  br label %for.cond

for.end19:                                        ; preds = %for.cond.cleanup
  ret void
}

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind willreturn }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 11.0.1-++20201103062553+ef4ffcafbb2-1~exp1~20201103053207.128"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
