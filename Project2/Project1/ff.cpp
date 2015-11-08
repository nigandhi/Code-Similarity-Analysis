void someScopes()
{
	for (size_t i = 0; i < 2; ++i)
	{
		if (true)
		{
			std::cout << "\n  will always get here";
			if (false)
				std::cout << "\n  won't ever get here";
		}
	}
}

