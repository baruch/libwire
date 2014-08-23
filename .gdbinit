set history filename .gdb_history
set history save
set print pretty on

set $wire_switched = 0

define list_len
 set $start = &$arg0
 set $count = 0
 set $i = $start.next
 while $count < 1000 && $i != $start
  set $count = $count+1
  set $i = $i.next
 end
 if $i == $start
  echo list size:
  p $count
 else
  echo size more than 1000\n
 end
end
document list_len
show the size of a list like list.h's
end

define list_iter_start
 set $li_start = &$arg0
 set $li_count = 0
 set $li_item = $li_start.next
 if $li_item == $li_start
   set $li_item = 0
 end
end
document list_iter_start
Setup iteration through the linked list, use list_iter_next to go through it
args: <list>
end

define list_iter_next
 if $li_item == 0
   print "End of list"
   return
 end
 set $li_item = $li_item.next
 if $li_item == $li_start
   set $li_item = 0
 end
end
document list_iter_next
Go through the list as setup by list_iter_start
args: <none>
result: $li_item
end

define list_print
 set $start = &$arg0
 set $count = 0
 set $i = $start.next
 while $count < 1000 && $i != $start
  p $arg1
  set $count = $count+1
  set $i = $i.next
 end
end
document list_print
print all elements in list
args: <list> <transform>
end

define list_print_cond
 set $start = &$arg0
 set $count = 0
 set $i = $start.next
 while $count < 1000 && $i != $start
  if $arg2
   p $arg1
  end
  set $count = $count+1
  set $i = $i.next
 end
end
document list_print_cond
print all elements in list conditionally
args: <list> <transform> <condition>
end

define list_find
 list_find_next $arg0 $arg1 $arg2 $arg0
end
document list_find
find element in a linked list like list.h's
args: <list> <transform> <condition>
transform is a transformation on list element $i
condition should use transformation result $x
end

define list_find_next
 set $start = &$arg0
 set $count = 0
 set $i = $arg3.next
 set $x = $arg1
 while $count < 1000 && $i != $start && !($arg2)
  set $count = $count+1
  set $i = $i.next
  set $x = $arg1
 end
 if $count == 1000 || $i == $start
  echo not found\n
 else
  echo found\n
  p $x
 end
end
document list_find_next
find next element in a linked list like list.h's
args: <list> <transform> <condition> <start>
transform is a transformation on list element $i
condition should use transformation result $x
end

def _struct_from_elem
 set $sfe = ($arg0 *) (((char*) $arg2) - (int) &(($arg0 *) 0).$arg1)
end

define struct_from_elem
 _struct_from_elem $arg0 $arg1 $arg2
 print $sfe
end
document struct_from_elem
get a struct from a pointer of its element.
args: <struct-type> <struct-elem-name> <elem-ptr>
end

define list_ready
 list_iter_start g_wire_thread.ready_list
 while $li_item != 0
   _struct_from_elem wire_t list $li_item
   printf "wire '%s': *(struct list_head*)0x%x  --  *(wire_t*) 0x%x\n", $sfe.name, $li_item, $sfe
   list_iter_next
 end
end
document list_ready
Display the ready task list
end

define list_suspend
 list_iter_start g_wire_thread.suspend_list
 while $li_item != 0
   _struct_from_elem wire_t list $li_item
   printf "wire '%s': *(struct list_head*)0x%x  --  *(wire_t*) 0x%x\n", $sfe.name, $li_item, $sfe
   list_iter_next
 end
end
document list_suspend
Display the suspended task list
end

define bt_wire
 set $wire = (wire_t*)$arg0
 set $new_rsp = $wire->ctx.sp

 set $old_rsp = $rsp
 set $old_rip = $rip
 set $old_rbp = $rbp

 set $rsp = $new_rsp
 set $rip = *(uint64_t *)($new_rsp + 6)
 set $rbp = *(uint64_t *)($new_rsp + 7)
 printf "Wire '%s' rip=0x%x:\n", $wire->name, $rip
 bt
 set $rsp = $old_rsp
 set $rip = $old_rip
 set $rbp = $old_rbp
end
document bt_wire
Display the backtrace of a wire
end

define switch_wire
 if $wire_switched != 1
   set $old_rsp1 = $rsp
   set $old_rip1 = $ip
   set $old_rbp1 = $rbp
   set $wire_switched = 1
 end

 set $wire = (wire_t*)$arg0
 set $new_rsp = $wire->ctx.sp
 set $rsp = $new_rsp
 set $rip = *(uint64_t *)($new_rsp + 6)
 set $rbp = *(uint64_t *)($new_rsp + 7)
 printf "Wire '%s' rip=0x%x:\n", $wire->name, $rip
 bt
end
document switch_wire
Switch to the wire given as an arg, use restore_wire to get back to the original place left
end

define restore_wire
 if $wire_switched == 1
   set $rsp = $old_rsp1
   set $rip = $old_rip1
   set $rbp = $old_rbp1
   set $wire_switched = 0
 end
end
document restore_wire
Switch back the original wire we left in switch_wire
end

define list_to_wire
 struct_from_elem wire_t list $arg0
end
document list_to_wire
Get the wire from its list element
args: <list_head>
end

define list_to_wire_i
 list_to_wire $i
end
document list_to_wire_i
Useful for list_find in the wire list, uses \$i as the argument
end
