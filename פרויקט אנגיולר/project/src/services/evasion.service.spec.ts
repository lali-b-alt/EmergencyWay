import { TestBed } from '@angular/core/testing';

import { EvasionService } from './evasion.service';

describe('EvasionService', () => {
  let service: EvasionService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(EvasionService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
