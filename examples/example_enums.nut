local e = WeekDay.SATURDAY;

print("The best day of the week is: " + e); // 5 -> SATURDAY

function getThirdDay(){
    return WeekDay.WEDNESDAY;
}

function isWednesday(day){
    return day == WeekDay.WEDNESDAY;
}
