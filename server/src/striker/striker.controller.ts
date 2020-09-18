import { Controller, Post, UseGuards } from '@nestjs/common';
import { StrikerService } from './striker.service';
import { JwtAuthGuard } from '../auth/jwt-auth.guard';

@Controller('striker')
export class StrikerController {
    constructor(private strikerService: StrikerService) {}

    @UseGuards(JwtAuthGuard)
    @Post()
    strike() {
        return this.strikerService.strike();
    }
}
