import { Component, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'app-keyboard',
  imports: [],
  templateUrl: './keyboard.component.html',
  styleUrl: './keyboard.component.css'
})

export class KeyboardComponent {
@Input()
 key!: string;

@Output() clickedEvent= new EventEmitter<string>();

onclick(){
  this.clickedEvent.emit(this.key)
}



}
