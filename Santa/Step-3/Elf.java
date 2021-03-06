import java.util.Random;

public class Elf implements Runnable {

	enum ElfState {
		WORKING, TROUBLE, AT_SANTAS_DOOR
	};

	private ElfState state;
	/**
	 * The number associated with the Elf
	 */
	private int number;
	private Random rand = new Random();
	private SantaScenario scenario;
	private boolean cancellationFlag;

	public Elf(int number, SantaScenario scenario) {
		this.number = number;
		this.scenario = scenario;
		this.state = ElfState.WORKING;
		this.cancellationFlag = false;
	}


	public ElfState getState() {
		return state;
	}

	/**
	 * Santa might call this function to fix the trouble
	 * @param state
	 */
	public void setState(ElfState state) {
		this.state = state;
	}

	public void threadCancel(){
		this.cancellationFlag = true;
	}



	@Override
	public void run() {
		while (!this.cancellationFlag) {
      		// wait a day
  			try {
  				Thread.sleep(100);
  			} catch (InterruptedException e) {
  				// TODO Auto-generated catch block
  				e.printStackTrace();
  			}
			switch (state) {
			case WORKING: {
				// at each day, there is a 1% chance that an elf runs into
				// trouble.
				if (rand.nextDouble() < 0.01) {
					state = ElfState.TROUBLE;
					while(scenario.lock){
					}
					scenario.lock = true;
					scenario.inTrouble++;
					scenario.lock = false;
				}
				break;
			}
			case TROUBLE:
				// FIXME: if possible, move to Santa's door
				while(scenario.lock){
				}
				scenario.lock = true;
				if(scenario.atSantasDoor > 0);
				else if( scenario.inTrouble >= 3){
					for(Elf elf : scenario.elves){
						if(elf.state == ElfState.TROUBLE){
							elf.state = ElfState.AT_SANTAS_DOOR;
							scenario.inTrouble--;
							if( ++scenario.atSantasDoor == 3 )
								break;
						}
					}	
				}
				scenario.lock = false;
				break;
			case AT_SANTAS_DOOR:
				// FIXME: if feasible, wake up Santa
				scenario.santa.wakeUp();
				break;
			}
		}
	}

	/**
	 * Report about my state
	 */
	public void report() {
		System.out.println("Elf " + number + " : " + state);
	}

}
