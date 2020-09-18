import { Module, HttpModule } from '@nestjs/common';
import { StrikerController } from './striker.controller';
import { StrikerService } from './striker.service';

@Module({
    imports: [HttpModule],
    providers: [StrikerService],
    controllers: [StrikerController],
})
export class StrikerModule {}
